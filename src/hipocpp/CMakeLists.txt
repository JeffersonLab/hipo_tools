cmake_minimum_required(VERSION 3.9)

add_library(hipocpp SHARED
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
set_target_properties(hipocpp PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(hipocpp PUBLIC ${LZ4_LIBRARY})

target_include_directories(hipocpp PUBLIC include)

install(TARGETS hipocpp
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp
    )
install(
    DIRECTORY
    include/hipo
    DESTINATION
    include
    FILES_MATCHING
    PATTERN "*.h"
)

configure_file(hipocpp.pc.in hipocpp.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/src/hipocpp/hipocpp.pc DESTINATION share/pkgconfig)
