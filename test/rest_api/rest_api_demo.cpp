#include <iostream>
#include <string>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace test_rest {


    void do_get_example() {
    spdlog::info("Starting GET example on localhost");

    cpr::Response response = cpr::Get(
        cpr::Url{"http://localhost:8080"},
        cpr::Header{{"Accept", "application/json"}}
    );

    std::cout << "=== GET example (localhost) ===\n";
    std::cout << "HTTP status: " << response.status_code << "\n";

    if (response.error) {
        std::cerr << "GET failed: " << response.error.message << "\n";
        return;
    }

if (response.status_code >= 200 && response.status_code < 300) {
    // 1. Debug: See what the server actually sent
    std::cout << "Raw Body: " << response.text << "\n";

    // try {
    //     json body = json::parse(response.text);
    //     std::cout << "message : " << body.value("message", "") << "\n";
    // } catch (json::parse_error& e) {
    //     std::cerr << "JSON Error: " << e.what() << "\n";
    // }
}
 else {
        std::cerr << "GET returned non-success status\n";
        std::cerr << response.text << "\n";
    }

    std::cout << "\n";
}


// void do_get_example() {
//     spdlog::info("Starting GET example");

//     cpr::Response response = cpr::Get(
//         cpr::Url{"https://jsonplaceholder.typicode.com/todos/1"},
//         cpr::Header{{"Accept", "application/json"}}
//     );

//     std::cout << "=== GET example ===\n";
//     std::cout << "HTTP status: " << response.status_code << "\n";

//     if (response.error) {
//         std::cerr << "GET failed: " << response.error.message << "\n";
//         return;
//     }

//     if (response.status_code >= 200 && response.status_code < 300) {
//         json body = json::parse(response.text);

//         std::cout << "userId    : " << body.value("userId", -1) << "\n";
//         std::cout << "id        : " << body.value("id", -1) << "\n";
//         std::cout << "title     : " << body.value("title", "") << "\n";
//         std::cout << "completed : " << std::boolalpha
//                   << body.value("completed", false) << "\n";
//     } else {
//         std::cerr << "GET returned non-success status\n";
//         std::cerr << response.text << "\n";
//     }

//     std::cout << "\n";
// }

void do_post_example() {
    spdlog::info("Starting POST example");

    json request_body = {
        {"title", "Learn REST with C++"},
        {"body", "This is a test_rest POST request"},
        {"userId", 101}
    };

    cpr::Response response = cpr::Post(
        cpr::Url{"https://jsonplaceholder.typicode.com/posts"},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Accept", "application/json"}
        },
        cpr::Body{request_body.dump()}
    );

    std::cout << "=== POST example ===\n";
    std::cout << "HTTP status: " << response.status_code << "\n";

    if (response.error) {
        std::cerr << "POST failed: " << response.error.message << "\n";
        return;
    }

    if (response.status_code >= 200 && response.status_code < 300) {
        json body = json::parse(response.text);

        std::cout << "Created resource:\n";
        std::cout << "id     : " << body.value("id", -1) << "\n";
        std::cout << "title  : " << body.value("title", "") << "\n";
        std::cout << "body   : " << body.value("body", "") << "\n";
        std::cout << "userId : " << body.value("userId", -1) << "\n";
    } else {
        std::cerr << "POST returned non-success status\n";
        std::cerr << response.text << "\n";
    }

    std::cout << "\n";
}

}  // namespace test_rest

int main() {
    try {
        test_rest::do_get_example();
        test_rest::do_post_example();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}