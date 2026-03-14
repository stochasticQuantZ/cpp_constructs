#include "elasticsearch/es_client.hpp"

#include <stdexcept>
#include <sstream>
#include <utility>
#include <cpr/cpr.h>

using json = nlohmann::json;

namespace {

json parse_or_throw(const cpr::Response& r) {
    if (r.error) {
        throw std::runtime_error("HTTP error: " + r.error.message);
    }

    if (r.status_code < 200 || r.status_code >= 300) {
        std::ostringstream oss;
        oss << "Elasticsearch returned status " << r.status_code
            << " with body: " << r.text;
        throw std::runtime_error(oss.str());
    }

    if (r.text.empty()) {
        return json::object();
    }

    return json::parse(r.text);
}

cpr::Header make_headers(const std::string& api_key) {
    return cpr::Header{
        {"Authorization", "ApiKey " + api_key},
        {"Accept", "application/json"},
        {"Content-Type", "application/json"}
    };
}

cpr::Header make_get_headers(const std::string& api_key) {
    return cpr::Header{
        {"Authorization", "ApiKey " + api_key},
        {"Accept", "application/json"}
    };
}

} // namespace

EsClient::EsClient(std::string base_url, std::string api_key)
    : base_url_(std::move(base_url)), api_key_(std::move(api_key)) {}

bool EsClient::ping() const {
    auto r = cpr::Get(
        cpr::Url{base_url_},
        make_get_headers(api_key_)
    );
    return !r.error && r.status_code == 200;
}

json EsClient::create_index(const std::string& index_name) const {
    json body = {
        {"mappings", {
            {"properties", {
                {"name", {{"type", "text"}}},
                {"category", {{"type", "keyword"}}},
                {"price", {{"type", "double"}}}
            }}
        }}
    };

    auto r = cpr::Put(
        cpr::Url{base_url_ + "/" + index_name},
        make_headers(api_key_),
        cpr::Body{body.dump()}
    );

    return parse_or_throw(r);
}

json EsClient::create_document(const std::string& index_name,
                               const std::string& doc_id,
                               const json& body) const {
    auto r = cpr::Post(
        cpr::Url{base_url_ + "/" + index_name + "/_doc/" + doc_id + "?refresh=true"},
        make_headers(api_key_),
        cpr::Body{body.dump()}
    );

    return parse_or_throw(r);
}

json EsClient::get_document(const std::string& index_name,
                            const std::string& doc_id) const {
    auto r = cpr::Get(
        cpr::Url{base_url_ + "/" + index_name + "/_doc/" + doc_id},
        make_get_headers(api_key_)
    );

    return parse_or_throw(r);
}

json EsClient::update_document(const std::string& index_name,
                               const std::string& doc_id,
                               const json& partial_doc) const {
    json body = {
        {"doc", partial_doc}
    };

    auto r = cpr::Post(
        cpr::Url{base_url_ + "/" + index_name + "/_update/" + doc_id + "?refresh=true"},
        make_headers(api_key_),
        cpr::Body{body.dump()}
    );

    return parse_or_throw(r);
}

json EsClient::delete_document(const std::string& index_name,
                               const std::string& doc_id) const {
    auto r = cpr::Delete(
        cpr::Url{base_url_ + "/" + index_name + "/_doc/" + doc_id + "?refresh=true"},
        make_get_headers(api_key_)
    );

    return parse_or_throw(r);
}

json EsClient::delete_index(const std::string& index_name) const {
    auto r = cpr::Delete(
        cpr::Url{base_url_ + "/" + index_name},
        make_get_headers(api_key_)
    );

    return parse_or_throw(r);
}