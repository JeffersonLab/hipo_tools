cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(include)

add_executable(dst2root src/dst2root.cpp)
target_link_libraries(dst2root
  PRIVATE hipocpp4_static
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(dst2root hipocpp4_static)
install(TARGETS dst2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)
