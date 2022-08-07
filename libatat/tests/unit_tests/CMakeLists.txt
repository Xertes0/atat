enable_testing()

add_executable(atat-tests)
set_property(TARGET atat-tests PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(atat-tests PRIVATE ${SOURCES})

target_link_libraries(atat-tests PRIVATE atat GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(atat-tests)