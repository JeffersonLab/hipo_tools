find_package(PythonInterp)
find_package(PythonLibs)
find_package(PythonExtensions)
find_package(Cython REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_cython_target(hipopy hipopy.pyx CXX PY3 OUTPUT_VAR _hipopy)
include_directories(${PYTHON_INCLUDE_DIRS})


set(EXTRA_FLAGS "-D__LZ4__ -pthread -m64")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )

add_library(hipopy MODULE ${_hipopy})
python_extension_module(hipopy)

target_link_libraries(hipopy hipocpp ${LZ4_LIBRARY} ${PYTHON_LIBRARIES})

install(TARGETS hipopy
        LIBRARY DESTINATION lib)
