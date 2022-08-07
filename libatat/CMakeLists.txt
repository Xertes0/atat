add_library(atat)

set_property(TARGET atat PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
target_sources(atat PRIVATE ${SOURCES})
target_include_directories(atat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(atat)

add_subdirectory(tests)

if(DOXYGEN)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    doxygen_add_docs(
        atat-docs
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ALL
    )
endif()