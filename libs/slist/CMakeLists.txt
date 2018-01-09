add_library(slist slist.cpp)

set_target_properties(slist 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_include_directories(slist INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)