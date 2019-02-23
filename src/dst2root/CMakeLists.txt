cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(dst2root_3 dst2root_3.cpp)
target_link_libraries(dst2root_3
  PRIVATE hipocpp3_static
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(dst2root_3 hipocpp3_static)
install(TARGETS dst2root_3
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)


add_executable(dst2root dst2root.cpp)
target_link_libraries(dst2root
  PRIVATE hipocpp4_static
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(dst2root hipocpp4_static)
install(TARGETS dst2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)
