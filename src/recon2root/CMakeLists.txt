cmake_minimum_required(VERSION 3.6)

add_executable(recon2root recon2root.cpp)

target_link_libraries(recon2root
  PRIVATE ${libname}
  PUBLIC hipocpp
  PUBLIC ${ROOT_LIBRARIES}
  )

add_dependencies(recon2root hipocpp)

install(TARGETS recon2root
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)


add_executable(recon2root_mc recon2root_mc.cpp)

target_link_libraries(recon2root_mc
  PRIVATE ${libname}
  PUBLIC hipocpp
  PUBLIC ${ROOT_LIBRARIES})

add_dependencies(recon2root_mc hipocpp)

install(TARGETS recon2root_mc
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)
