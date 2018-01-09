add_library(liballocator Chunk.cpp FixedAllocator.cpp SmallObjAllocator.cpp Allocator.cpp)

set_target_properties(liballocator 
    PROPERTIES 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_include_directories(liballocator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)