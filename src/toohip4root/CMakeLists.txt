cmake_minimum_required(VERSION 3.6)
enable_testing()

#SET(lib_VERSION "${${PROJECT_NAME}_VERSION}")
#SET(lib_MAJOR_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}")
#SET(lib_LIBRARY_PROPERTIES 
#    VERSION "${lib_VERSION}"
#    SOVERSION "${lib_MAJOR_VERSION}"
#    SUFFIX ".so")

set(HEADERS 
  include/THipo.h
  include/THipoItem.h
  include/THipoBank.h
  include/THipoBankParser.h
  #include/RHipoDS.h
  )

ROOT_GENERATE_DICTIONARY(toohip4rootDict 
  -I${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS} 
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h
  OPTIONS -p)
ADD_CUSTOM_TARGET(toohip4rootDict_ROOTDICTS DEPENDS ${HEADERS} toohip4rootDict.cxx )

message("ROOT_LIBRARIES ${ROOT_LIBRARIES} ")
add_library(hiporoot SHARED 
  src/THipo.cxx
  src/THipoItem.cxx
  src/THipoBank.cxx
  src/THipoBankParser.cxx
  toohip4rootDict.cxx
  #src/RHipoDS.cxx
  )
target_link_libraries(hiporoot 
  PUBLIC ${ROOT_LIBRARIES} 
  PUBLIC ${LZ4_LIBRARY}
  PRIVATE hipocpp
  )
#add_dependencies(hiporoot hipocpp)
#set_target_properties(hiporoot PROPERTIES ${lib_LIBRARY_PROPERTIES})
#target_compile_features(${libname} PUBLIC cxx_std_14)
#add_dependencies(${libname} ${needs_libs} ${aname}_ROOTDICTS)
target_include_directories(hiporoot 
  PUBLIC include
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

install(
  TARGETS hiporoot 
  EXPORT ${PROJECT_NAME}Targets
  DESTINATION lib )

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/toohip4rootDict_rdict.pcm 
  DESTINATION lib )

SET(lib_HEADERS ${HEADERS} ${template_headers})
install(
  FILES ${HEADERS} 
  DESTINATION include/hipo )

#install(TARGETS foo
#  # IMPORTANT: Add the foo library to the "export-set"
#  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
#  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
#  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/foo"
#    COMPONENT dev)

add_executable(toohip4root src/toohip4root.cxx)
#target_compile_features(toohipforroot 
#  PRIVATE cxx_std_17)
target_include_directories(toohip4root 
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  )
  #  PRIVATE ${libname} stdc++fs
target_link_libraries(toohip4root
  PRIVATE ${libname} ${CXX_FILESYSTEM_LIB}
  PUBLIC hiporoot
  PUBLIC ${ROOT_LIBRARIES} 
  )
add_dependencies(toohip4root hipocpp)
install(TARGETS toohip4root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)

#set(test_name test_hipo)
#add_executable(${test_name} test/${test_name}.cxx)
#target_include_directories(${test_name} PUBLIC
#  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
#  )
#target_link_libraries(${test_name}
#  PUBLIC ${libname}
#  )
#add_test(NAME ${test_name} COMMAND ${test_name} -s)

