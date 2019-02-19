cmake_minimum_required(VERSION 3.5)

add_executable(recon2root recon2root.cpp)

target_link_libraries(recon2root
  PRIVATE ${libname}
  PUBLIC hipocpp3
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(recon2root hipocpp3)

install(TARGETS recon2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)


add_executable(recon2root_mc recon2root_mc.cpp)

target_link_libraries(recon2root_mc
  PRIVATE ${libname}
  PUBLIC hipocpp3
  PUBLIC ${ROOT_LIBRARIES})

add_dependencies(recon2root_mc hipocpp3)

install(TARGETS recon2root_mc
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)
