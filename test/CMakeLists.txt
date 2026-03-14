add_executable(test_singleton test_singleton.cpp)
target_link_libraries(test_singleton PRIVATE 
    singleton
    )

add_executable(test_linear_search test_search.cpp)
target_link_libraries(test_linear_search
    linear_search
    gtest_main
)
add_executable(test_cpp test_cpp.cpp)

target_link_libraries(test_cpp PUBLIC
    gtest_main
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
    # grpc::grpc++
    # Protobuf::Protobuf
)
target_include_directories(test_cpp
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Existing tests can stay here too

add_executable(test_permutation_server
  gRPC/server.cpp
)

target_link_libraries(test_permutation_server
  PRIVATE
    cpp_constructs::permutation_proto
    spdlog::spdlog
)

add_executable(test_permutation_client
  gRPC/client.cpp
)

target_link_libraries(test_permutation_client
  PRIVATE
    cpp_constructs::permutation_proto
    spdlog::spdlog
)



# Existing test targets can stay here

add_executable(rest_api_demo
  rest_api/rest_api_demo.cpp
)

target_link_libraries(rest_api_demo
  PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

add_executable(test_rest_api
  rest_api/test_rest_api.cpp
)

target_link_libraries(test_rest_api
  PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(test_rest_api)

add_test(NAME RestApiDemoRuns COMMAND rest_api_demo)

add_executable(rest_server
  rest_impl.cpp
)

target_include_directories(rest_server PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(rest_server PRIVATE
  nlohmann_json::nlohmann_json
  httplib::httplib
)

if(APPLE)
  target_link_libraries(rest_server PRIVATE
    "-framework CoreFoundation"
    "-framework CFNetwork"
  )
endif()
# # Optional GoogleTest-based test
# add_executable(test_permutation
#   gRPC/test_permutation.cpp
# )

# target_link_libraries(test_permutation
#   PRIVATE
#     cpp_constructs::permutation_proto
#     GTest::gtest_main
# )

# include(GoogleTest)
# gtest_discover_tests(test_permutation)



add_executable(database_setup
    database/database_setup.cpp
)

target_include_directories(database_setup PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(database_setup PRIVATE
    sqlite_db
)

add_executable(database_test
    database/database_test.cpp
)

target_include_directories(database_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(database_test PRIVATE
    sqlite_db
)


add_subdirectory(es)
