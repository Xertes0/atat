cmake_minimum_required(VERSION 3.18)
project(atat_em CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

option(BUILD_TESTS "Build tests. Every test will not be build otherwise" OFF)
option(CPUDIAG_TEST "Build cpudiag test" ON)

option(DOXYGEN "Build doxygen documentation" OFF)

option(MEMORY_CHECKS "Enable memory checks" ON)

if(NOT BUILD_TESTS)
    set(UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(CPUDIAG_TEST OFF CACHE BOOL "" FORCE)
endif()

if(MEMORY_CHECKS)
    add_compile_definitions(MEMORY_CHECKS)
endif()

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
