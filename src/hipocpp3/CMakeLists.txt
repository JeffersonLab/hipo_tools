cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")

find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

add_library(hipocpp3 SHARED
  src/data.cpp
  src/dictionary.cpp
  src/event.cpp
  src/node.cpp
  src/reader.cpp
  src/record.cpp
  src/text.cpp
  src/utils.cpp
  src/wrapper.cpp
  src/writer.cpp
  )


target_link_libraries(hipocpp3
  PUBLIC ${LZ4_LIBRARY}
  )

target_include_directories(hipocpp3
  PRIVATE include)

install(TARGETS hipocpp3
  EXPORT HipoToolsTargets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp3
    )


install(
    DIRECTORY
    include/hipo3
    DESTINATION
    include
    FILES_MATCHING
    PATTERN "*.h"
)

configure_file(hipocpp.pc.in hipocpp3.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/src/hipocpp3/hipocpp3.pc DESTINATION share/pkgconfig)
