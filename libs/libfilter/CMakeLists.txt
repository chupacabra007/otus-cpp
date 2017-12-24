add_library(libfilter filter.cpp)

set_target_properties(libfilter  
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_include_directories(libfilter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)