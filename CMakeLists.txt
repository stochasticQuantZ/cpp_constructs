cmake_minimum_required(VERSION 3.22)
project(
  cpp_constructs
  VERSION 1.0.0
  LANGUAGES C CXX
  )

set(CMAKE_CXX_STANDARD 17)


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

# Library

add_subdirectory(src)
add_subdirectory(configured)

if (TEST_EXEC_REQUIRED)
  add_subdirectory(test)
else()
  message("-----------Without creating executable ----------")
endif()

# Tests
#enable_testing()
#add_test(NAME SingletonTest COMMAND test_singleton)

