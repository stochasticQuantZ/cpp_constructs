#include "elasticsearch/es_client.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

int main() {
    try {
        const char* api_key_env = std::getenv("ES_API_KEY");
        if (api_key_env == nullptr || std::string(api_key_env).empty()) {
            std::cerr << "ES_API_KEY is not set\n";
            std::cerr << "Run: export ES_API_KEY='your_api_key_here'\n";
            return 1;
        }

        const std::string api_key = api_key_env;

        EsClient client("http://localhost:9200", api_key);

        const std::string index_name = "products_cpp_test";
        const std::string doc_id = "1";

        if (!client.ping()) {
            std::cerr << "Elasticsearch is not reachable at http://localhost:9200\n";
            return 1;
        }

        std::cout << "Connected to Elasticsearch\n";

        // Clean up from earlier runs if index exists
        try {
            auto deleted = client.delete_index(index_name);
            std::cout << "Deleted old index:\n" << deleted.dump(2) << "\n\n";
        } catch (...) {
            std::cout << "No old index to delete. Continuing...\n";
        }

        // CREATE INDEX
        auto create_index_resp = client.create_index(index_name);
        std::cout << "Create index response:\n" << create_index_resp.dump(2) << "\n\n";

        // CREATE DOCUMENT
        json doc = {
            {"name", "Laptop"},
            {"category", "electronics"},
            {"price", 999.99}
        };

        auto create_doc_resp = client.create_document(index_name, doc_id, doc);
        std::cout << "Create document response:\n" << create_doc_resp.dump(2) << "\n\n";

        // READ DOCUMENT
        auto get_doc_resp = client.get_document(index_name, doc_id);
        std::cout << "Get document response:\n" << get_doc_resp.dump(2) << "\n\n";

        // UPDATE DOCUMENT
        json partial_update = {
            {"price", 899.99}
        };

        auto update_doc_resp = client.update_document(index_name, doc_id, partial_update);
        std::cout << "Update document response:\n" << update_doc_resp.dump(2) << "\n\n";

        // READ AGAIN
        auto get_after_update = client.get_document(index_name, doc_id);
        std::cout << "Get after update:\n" << get_after_update.dump(2) << "\n\n";

        // DELETE DOCUMENT
        auto delete_doc_resp = client.delete_document(index_name, doc_id);
        std::cout << "Delete document response:\n" << delete_doc_resp.dump(2) << "\n\n";

        // DELETE INDEX
        auto delete_index_resp = client.delete_index(index_name);
        std::cout << "Delete index response:\n" << delete_index_resp.dump(2) << "\n\n";

        std::cout << "CRUD test completed successfully\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Test failed: " << ex.what() << "\n";
        return 1;
    }
}