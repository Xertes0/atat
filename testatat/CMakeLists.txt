enable_testing()

add_executable(atat-test)
set_property(TARGET atat-test PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(atat-test PRIVATE ${SOURCES})

target_link_libraries(atat-test PRIVATE atat GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(atat-test)