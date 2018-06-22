cmake_minimum_required(VERSION 3.6)
project(hipoio VERSION 0.0.1 DESCRIPTION "hipo-io")

set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake
   ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS Tree)
include(${ROOT_USE_FILE})
#add_definitions(${ROOT_CXX_FLAGS})
## extra root settings
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})
include(cmake/root.cmake)


add_subdirectory(src/hipocpp)
include_directories(src/hipocpp/include)
add_subdirectory(src/toohip4root)
include_directories(src/hipocpp/include/hipo)
add_subdirectory(src/hipo2root)

add_executable(hipo examples/hipo.cpp)
target_link_libraries(hipo hipocpp ${LZ4_LIBRARY})
install(TARGETS hipo RUNTIME DESTINATION bin)

