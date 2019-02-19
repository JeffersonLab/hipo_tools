cmake_minimum_required(VERSION 3.6)

file(GLOB EXAMPLES hipo3/*.cpp )
foreach(exe ${EXAMPLES} )
    get_filename_component(exename ${exe} NAME)
    string( REPLACE ".cpp" "" exename ${exename} )
    add_executable( ${exename} ${exe} )
    target_link_libraries(${exename}
      PRIVATE ${libname}
      PUBLIC hipocpp3_static
      PUBLIC ${ROOT_LIBRARIES})
    add_dependencies(${exename} hipocpp3_static)
    install(TARGETS ${exename}
      EXPORT ${PROJECT_NAME}Targets
      RUNTIME DESTINATION bin/hipo3_examples)
endforeach( exe ${EXAMPLES} )


file(GLOB EXAMPLES hipo4/*.cpp )
foreach(exe ${EXAMPLES} )
    get_filename_component(exename ${exe} NAME)
    string( REPLACE ".cpp" "" exename ${exename} )
    add_executable( ${exename} ${exe} )
    target_link_libraries(${exename}
      PRIVATE ${libname}
      PUBLIC hipocpp4_static
      PUBLIC ${ROOT_LIBRARIES})
    add_dependencies(${exename} hipocpp4_static)
    install(TARGETS ${exename}
      EXPORT ${PROJECT_NAME}Targets
      RUNTIME DESTINATION bin/hipo4_examples)
endforeach( exe ${EXAMPLES} )


file(GLOB EXAMPLES dst2root/*.cxx )
foreach(exe ${EXAMPLES} )
    get_filename_component(exename ${exe} NAME)
    string( REPLACE ".cxx" "" exename ${exename} )
    add_executable( ${exename} ${exe} )
    target_link_libraries(${exename}
      PUBLIC ${ROOT_LIBRARIES})
    install(TARGETS ${exename}
      EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION bin/dst2root_examples)
endforeach( exe ${EXAMPLES} )
