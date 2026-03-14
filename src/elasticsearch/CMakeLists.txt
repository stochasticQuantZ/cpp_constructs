add_library(es_client
    es_client.cpp
)

target_include_directories(es_client PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(es_client PUBLIC
    cpr::cpr
    nlohmann_json::nlohmann_json
)