find_package(PythonInterp)
find_package(PythonLibs)
find_package(PythonExtensions)
find_package(Cython REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_cython_target(hipopy3 hipopy3.pyx CXX PY3 OUTPUT_VAR _hipopy3)
include_directories(${PYTHON_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )
set(EXTRA_FLAGS "-D__LZ4__ -fPIC -pthread -m64")

add_library(hipopy3 MODULE ${_hipopy3})
python_extension_module(hipopy3)

target_link_libraries(hipopy3 hipocpp3 ${LZ4_LIBRARY} ${PYTHON_LIBRARIES})
install(TARGETS hipopy3 LIBRARY DESTINATION lib)
