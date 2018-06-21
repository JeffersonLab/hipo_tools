cmake_minimum_required(VERSION 3.6)
project(hipoio VERSION 0.0.1 DESCRIPTION "hipo-io")

set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake
   ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

add_subdirectory(src/hipocpp)

add_executable(hipo examples/hipo.cpp)
target_link_libraries(hipo hipocpp ${LZ4_LIBRARY})
install(TARGETS hipo RUNTIME DESTINATION bin)

