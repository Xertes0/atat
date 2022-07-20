add_executable(atat-main)

set_property(TARGET atat-main PROPERTY CXX_STANDARD 20)

target_sources(
    atat-main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
)

target_link_libraries(atat-main PRIVATE atat)