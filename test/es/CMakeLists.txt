add_executable(es_crud_test
    database_test.cpp
)

target_include_directories(es_crud_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(es_crud_test PRIVATE
    es_client
)