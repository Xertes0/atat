add_executable(cpudiag)

set_property(TARGET cpudiag PROPERTY CXX_STANDARD 20)

target_sources(cpudiag PRIVATE src/main.cc)

target_link_libraries(cpudiag PRIVATE atat)