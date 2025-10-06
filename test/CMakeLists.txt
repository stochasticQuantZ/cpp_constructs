add_executable(test_singleton test_singleton.cpp)
target_link_libraries(test_singleton PRIVATE 
    singleton
    )
