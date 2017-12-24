add_executable(ip_filter main.cpp)

set_target_properties(ip_filter 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(
    ip_filter libfilter
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)