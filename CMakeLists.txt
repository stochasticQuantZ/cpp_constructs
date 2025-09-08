cmake_minimum_required(VERSION 3.16)
project(cpp_constructs)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)



# Library
add_library(singleton src/singleton.cpp)
target_include_directories(singleton PUBLIC include)
target_link_libraries(singleton PUBLIC spdlog::spdlog)


# Tests
enable_testing()
add_executable(test_singleton test/test_singleton.cpp)
target_link_libraries(test_singleton PRIVATE singleton)
add_test(NAME SingletonTest COMMAND test_singleton)

