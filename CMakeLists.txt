cmake_minimum_required(VERSION 4.0)
project(
  cpp_constructs
  VERSION 1.0.0
  LANGUAGES C CXX
  )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")


include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

option(TEST_EXEC_REQUIRED "Compile the test as well!!" ON)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(AddGitSubmodule)

add_git_submodule(github.com/json)


FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  FMT
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 12.0.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(FMT)


# --- GoogleTest ---
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)  # don't install gtest

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

# # Find the installed Protobuf package
# find_package(Protobuf CONFIG REQUIRED)

# # Find the installed gRPC package
# find_package(gRPC CONFIG REQUIRED)

# Library

add_subdirectory(src)
add_subdirectory(configured)

if (TEST_EXEC_REQUIRED)
  add_subdirectory(test)
else()
  message("-----------Without creating executable ----------")
endif()

# Tests
enable_testing()
add_test(NAME SingletonTest COMMAND test_singleton)
add_test(NAME LinearSearchTest001 COMMAND test_linear_search)
add_test(NAME Test_Cpp_Features COMMAND test_cpp)