add_executable(libfilter_test libfilter_test.cpp)

set_target_properties(libfilter_test 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(
    libfilter_test ${Boost_LIBRARIES} libfilter
)

add_test(libfilter_test libfilter_test)