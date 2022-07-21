cmake_minimum_required(VERSION 3.20)
project(atat_em CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

option(UNIT_TESTS "Build unit tests" ON)
option(DOXYGEN "Build doxygen documentation" OFF)

if(UNIT_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

if(DOXYGEN)
    find_package(Doxygen REQUIRED)
endif()

add_subdirectory(libatat)
add_subdirectory(atat)