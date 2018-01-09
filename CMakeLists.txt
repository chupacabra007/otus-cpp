cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.0) #$ENV{TRAVIS_BUILD_NUMBER}

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_subdirectory(libs/liballocator)
add_subdirectory(libs/slist)
add_subdirectory(app/allocator)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT vv_volkov@mail.ru)
include(CPack)

enable_testing()
add_subdirectory(tests/allocator)