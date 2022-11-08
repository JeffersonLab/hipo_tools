cmake_minimum_required(VERSION 3.5)

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

if (BUILD_EXAMPLES)
    add_executable(recon2root recon2root.cpp)
    add_dependencies(recon2root hipocpp4_static)
    target_link_libraries(recon2root
            PRIVATE ${libname}
            PUBLIC hipocpp4_static
            PUBLIC ${ROOT_LIBRARIES}
            )
    install(TARGETS recon2root
            EXPORT ${PROJECT_NAME}Targets
            RUNTIME DESTINATION bin)
endif ()

