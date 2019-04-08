cmake_minimum_required(VERSION 3.5)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(hipo4_srcs
  src/bank.cpp
  src/dictionary.cpp
  src/event.cpp
  src/reader.cpp
  src/record.cpp
  src/recordbuilder.cpp
  src/utils.cpp
  )

add_library(hipo4_objlib OBJECT ${hipo4_srcs})
set_property(TARGET hipo4_objlib PROPERTY POSITION_INDEPENDENT_CODE ON)

# shared and static libraries built from the same object files
add_library(hipocpp4 SHARED $<TARGET_OBJECTS:hipo4_objlib>)
add_library(hipocpp4_static STATIC $<TARGET_OBJECTS:hipo4_objlib>)

target_link_libraries(hipocpp4 PUBLIC ${LZ4_LIBRARY})
target_link_libraries(hipocpp4_static PUBLIC ${LZ4_LIBRARY})

target_include_directories(hipocpp4 PRIVATE include)

install(TARGETS hipocpp4
  EXPORT HipoToolsTargets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp4
    )

install(TARGETS hipocpp4_static
  EXPORT HipoToolsTargets
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp4
    )

install(
    DIRECTORY
    include/hipo4
    DESTINATION
    include
    FILES_MATCHING
    PATTERN "*.h"
)

configure_file(hipocpp.pc.in hipocpp4.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/src/hipocpp4/hipocpp4.pc DESTINATION share/pkgconfig)
