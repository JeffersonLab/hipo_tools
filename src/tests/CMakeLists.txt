cmake_minimum_required(VERSION 3.5)

add_executable(pz_test pz_test.cpp)
target_link_libraries(pz_test
    PRIVATE ${libname}
    PUBLIC hipocpp4
    PUBLIC ${ROOT_LIBRARIES}
    )
add_dependencies(pz_test hipocpp4)
install(TARGETS pz_test
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION bin)
