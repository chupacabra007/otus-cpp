add_executable(libmatrix_test libmatrix_test.cpp)

set_target_properties(libmatrix_test 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(
    libmatrix_test ${Boost_LIBRARIES} libmatrix
)

add_test(libmatrix_test libmatrix_test)