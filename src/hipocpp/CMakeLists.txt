cmake_minimum_required(VERSION 3.9)

#file(GLOB hipo_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/hipo/*.h)
#
#ROOT_GENERATE_DICTIONARY_ROOTMAP(hipocppDict libhipocpp
#  -I${PROJECT_SOURCE_DIR}/src/Hipo -I${PROJECT_SOURCE_DIR}/src/Hipo/include ${hipo_headers} 
#  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h 
#  OPTIONS -p)
#
#ADD_CUSTOM_TARGET(hipocppDict_ROOTDICTS 
#  DEPENDS hipocppDict.cxx  )


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
  #  hipocppDict.cxx 
  )
set_target_properties(hipocpp PROPERTIES VERSION ${PROJECT_VERSION})
#add_dependencies(hipocpp hipocppDict_ROOTDICTS)

target_link_libraries(hipocpp PUBLIC ${LZ4_LIBRARY} ${ROOT_LIBRARIES})

target_include_directories(hipocpp 
  PRIVATE include)

install(TARGETS hipocpp
  EXPORT HipoToolsTargets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/hipocpp
    )
  #install(
  #  FILES 
  #  ${CMAKE_CURRENT_BINARY_DIR}/hipocppDict_rdict.pcm 
  #  ${CMAKE_CURRENT_BINARY_DIR}/libhipocpp.rootmap
  #  DESTINATION lib )

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

