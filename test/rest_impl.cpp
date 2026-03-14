#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "combinations.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

bool is_valid_filename(const std::string& name)
{
    if (name.empty()) {
        return false;
    }

    static const std::regex pattern(R"(^[A-Za-z0-9._-]+$)");
    return std::regex_match(name, pattern);
}

int main()
{
    httplib::Server server;

    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        json response = {
            {"message", "C++ REST server running on localhost"},
            {"endpoints", {
                "GET /combinations?input=abc",
                "POST /files  {\"name\":\"sample.txt\"}"
            }}
        };
        res.set_content(response.dump(4), "application/json");
    });

    server.Get("/combinations", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("input")) {
            res.status = 400;
            res.set_content(
                json{{"error", "Missing query parameter: input"}}.dump(4),
                "application/json");
            return;
        }

        std::string input = req.get_param_value("input");
        auto combinations = get_all_combinations(input);

        std::sort(combinations.begin(), combinations.end(),
                  [](const std::string& a, const std::string& b) {
                      if (a.size() == b.size()) {
                          return a < b;
                      }
                      return a.size() < b.size();
                  });

        json response = {
            {"input", input},
            {"count", combinations.size()},
            {"combinations", combinations}
        };

        res.set_content(response.dump(4), "application/json");
    });

    server.Post("/files", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);

            if (!body.contains("name") || !body["name"].is_string()) {
                res.status = 400;
                res.set_content(
                    json{{"error", "JSON field 'name' is required and must be a string"}}.dump(4),
                    "application/json");
                return;
            }

            std::string file_name = body["name"].get<std::string>();

            if (!is_valid_filename(file_name)) {
                res.status = 400;
                res.set_content(
                    json{{"error", "Invalid file name"}}.dump(4),
                    "application/json");
                return;
            }

            fs::path output_dir = "test/created_files";
            fs::create_directories(output_dir);

            fs::path file_path = output_dir / file_name;

            if (fs::exists(file_path)) {
                res.status = 409;
                res.set_content(
                    json{{"error", "File already exists"},
                         {"path", file_path.string()}}.dump(4),
                    "application/json");
                return;
            }

            std::ofstream out(file_path);
            if (!out) {
                res.status = 500;
                res.set_content(
                    json{{"error", "Failed to create file"}}.dump(4),
                    "application/json");
                return;
            }

            out << "Dummy file created by POST /files\n";
            out.close();

            json response = {
                {"message", "File created successfully"},
                {"path", file_path.string()}
            };

            res.status = 201;
            res.set_content(response.dump(4), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(
                json{{"error", "Invalid JSON body"}}.dump(4),
                "application/json");
        }
    });

    std::cout << "Server running at http://127.0.0.1:8080\n";
    server.listen("127.0.0.1", 8080);

    return 0;
}