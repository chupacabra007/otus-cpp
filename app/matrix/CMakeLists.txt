add_executable(matrix main.cpp)

set_target_properties(matrix
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(
    matrix libmatrix
)

install(TARGETS matrix RUNTIME DESTINATION bin)