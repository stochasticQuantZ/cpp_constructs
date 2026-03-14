add_library(singleton singleton.cpp le-be_test.cpp)
target_link_libraries(singleton PUBLIC 
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json)
target_include_directories(singleton PUBLIC 
    "../include"
    #"../github.com/json/include"
    "${CMAKE_BINARY_DIR}/configured_files/include"    
)

add_library(linear_search linear_search.cpp)
target_link_libraries(linear_search 
    
    PUBLIC 
    
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
    
    )
target_include_directories(linear_search
    PUBLIC 
    "../include")


add_library(sqlite_db
    database/sqlite_db.cpp
)

target_include_directories(sqlite_db PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(sqlite_db PRIVATE
    ${SQLite3_LIBRARIES}
)


add_subdirectory(elasticsearch)