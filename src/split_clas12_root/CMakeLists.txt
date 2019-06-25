cmake_minimum_required(VERSION 3.5)

add_executable(split_clas12 src/split_clas12.cpp)
target_link_libraries(split_clas12
  PUBLIC ${ROOT_LIBRARIES}
  )

install(TARGETS split_clas12
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin)

