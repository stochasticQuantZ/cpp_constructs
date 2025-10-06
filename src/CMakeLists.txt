add_library(singleton singleton.cpp le-be_test.cpp)
target_link_libraries(singleton PUBLIC spdlog::spdlog)
target_include_directories(singleton PUBLIC 
    "../include"
    "../github.com/json/include"
    "${CMAKE_BINARY_DIR}/configured_files/include"    
)
