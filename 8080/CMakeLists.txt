add_executable(8080)

set_property(TARGET 8080 PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(8080 PRIVATE ${SOURCES})
target_include_directories(8080 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(8080)
