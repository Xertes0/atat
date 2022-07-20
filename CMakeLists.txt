cmake_minimum_required(VERSION 3.20)
project(atat_em CXX)

option(UNIT_TESTS "Enable unit tests" ON)

if(UNIT_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(libatat)
add_subdirectory(atat)

if(UNIT_TESTS)
    add_subdirectory(testatat)
endif()