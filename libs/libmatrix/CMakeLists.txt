add_library(libmatrix SpareMatrix.cpp)

set_target_properties(libmatrix
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_include_directories(libmatrix INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)