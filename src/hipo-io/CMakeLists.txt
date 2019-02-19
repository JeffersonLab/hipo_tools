cmake_minimum_required(VERSION 3.5)
enable_testing()

add_executable(hipo_io src/hipo.cxx)
#target_compile_features(toohipforroot
#  PRIVATE cxx_std_17)
target_include_directories(hipo_io
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  )
  #PRIVATE ${libname} stdc++fs
target_link_libraries(hipo_io
  PRIVATE ${libname} ${CXX_FILESYSTEM_LIB}
  PUBLIC hipocpp
  )

install(TARGETS hipo_io
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)

# -------------------------------------
# tests
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
