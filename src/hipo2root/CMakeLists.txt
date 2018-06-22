cmake_minimum_required(VERSION 3.6)

add_executable(hipo2root hipo2root.cpp)

target_link_libraries(hipo2root
  PRIVATE ${libname}
  PUBLIC hipocpp
  PUBLIC ${ROOT_LIBRARIES} 
  )

add_dependencies(hipo2root hipocpp)

install(TARGETS hipo2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)

