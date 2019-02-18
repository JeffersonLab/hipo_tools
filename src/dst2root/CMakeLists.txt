cmake_minimum_required(VERSION 3.6)

add_executable(dst2root dst2root.cpp)
target_link_libraries(dst2root
  PRIVATE ${libname}
  PUBLIC hipocpp4_static
  PUBLIC ${ROOT_LIBRARIES}
  )
  
add_dependencies(dst2root hipocpp4_static)
install(TARGETS dst2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)
