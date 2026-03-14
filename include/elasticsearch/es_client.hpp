#pragma once

#include <string>
#include <nlohmann/json.hpp>

class EsClient {
public:
    EsClient(std::string base_url, std::string api_key);

    bool ping() const;

    nlohmann::json create_index(const std::string& index_name) const;
    nlohmann::json create_document(const std::string& index_name,
                                   const std::string& doc_id,
                                   const nlohmann::json& body) const;
    nlohmann::json get_document(const std::string& index_name,
                                const std::string& doc_id) const;
    nlohmann::json update_document(const std::string& index_name,
                                   const std::string& doc_id,
                                   const nlohmann::json& partial_doc) const;
    nlohmann::json delete_document(const std::string& index_name,
                                   const std::string& doc_id) const;
    nlohmann::json delete_index(const std::string& index_name) const;

private:
    std::string base_url_;
    std::string api_key_;
};