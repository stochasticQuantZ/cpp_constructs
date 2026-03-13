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