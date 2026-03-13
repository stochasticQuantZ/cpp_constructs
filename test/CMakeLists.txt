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