cmake_minimum_required(VERSION 3.5)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(hipo3_srcs
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


add_library(hipo3_objlib OBJECT ${hipo3_srcs})
set_property(TARGET hipo3_objlib PROPERTY POSITION_INDEPENDENT_CODE ON)

# shared and static libraries built from the same object files
add_library(hipocpp3 SHARED $<TARGET_OBJECTS:hipo3_objlib>)
add_library(hipocpp3_static STATIC $<TARGET_OBJECTS:hipo3_objlib>)

target_link_libraries(hipocpp3 PUBLIC ${LZ4_LIBRARY})
target_link_libraries(hipocpp3_static PUBLIC ${LZ4_LIBRARY})

target_include_directories(hipocpp3 PRIVATE include)

install(TARGETS hipocpp3
  EXPORT HipoToolsTargets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp3
    )

install(TARGETS hipocpp3_static
  EXPORT HipoToolsTargets
    ARCHIVE DESTINATION lib
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
