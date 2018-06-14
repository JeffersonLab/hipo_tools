cmake_minimum_required(VERSION 3.9)
project(hipoio VERSION 0.0.1 DESCRIPTION "hipo-io")

add_subdirectory(hipocpp)

add_executable(hipo examples/hipo.cpp)
target_link_libraries(hipo hipocpp)

configure_file(hipocpp/hipocpp.pc.in hipocpp.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/hipocpp.pc DESTINATION share/pkgconfig)
