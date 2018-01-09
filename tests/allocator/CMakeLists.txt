add_executable(allocator_test allocator_test.cpp)

set_target_properties(allocator_test 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(
    allocator_test ${Boost_LIBRARIES} liballocator slist
)

add_test(allocator_test allocator_test)