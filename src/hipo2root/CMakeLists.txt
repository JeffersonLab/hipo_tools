cmake_minimum_required(VERSION 3.5)

add_executable(hipo2root_3 hipo2root3.cpp)

target_link_libraries(hipo2root_3
  PRIVATE ${libname}
  PUBLIC hipocpp3_static
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(hipo2root_3 hipocpp3_static)

install(TARGETS hipo2root_3
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)


add_executable(hipo2root hipo2root.cpp)

target_link_libraries(hipo2root
    PRIVATE ${libname}
    PUBLIC hipocpp4_static
    PUBLIC ${ROOT_LIBRARIES}
    )

add_dependencies(hipo2root hipocpp4_static)

install(TARGETS hipo2root
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION bin)
