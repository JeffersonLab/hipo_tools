find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)
find_package(NumPy REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_cython_target(hipopy3 hipopy3.pyx CXX PY3 OUTPUT_VAR _hipopy3)
include_directories(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR} ${NumPy_INCLUDE_DIR})
set(EXTRA_FLAGS "-D__LZ4__ -pthread -m64")
set(CMAKE_CXX_FLAGS  "-Wno-deprecated-register ${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )
add_library(hipopy3 MODULE ${_hipopy3})
python_extension_module(hipopy3)
target_link_libraries(hipopy3 hipocpp3_static ${LZ4_LIBRARY} ${PYTHON_LIBRARIES} ${ROOT_LIBRARIES})
install(TARGETS hipopy3
        LIBRARY DESTINATION lib)

add_cython_target(hipopy4 hipopy4.pyx CXX PY3 OUTPUT_VAR _hipopy4)
include_directories(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR} ${NumPy_INCLUDE_DIR})
set(EXTRA_FLAGS "-D__LZ4__ -pthread -m64")
set(CMAKE_CXX_FLAGS  "-Wno-deprecated-register ${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )
add_library(hipopy4 MODULE ${_hipopy4})
python_extension_module(hipopy4)
target_link_libraries(hipopy4 hipocpp4_static ${LZ4_LIBRARY} ${PYTHON_LIBRARIES} ${ROOT_LIBRARIES})
install(TARGETS hipopy4
        LIBRARY DESTINATION lib)

add_cython_target(clas12 clas12.pyx CXX PY3 OUTPUT_VAR _clas12)
include_directories(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR} ${NumPy_INCLUDE_DIR})
set(EXTRA_FLAGS "-D__LZ4__ -pthread -m64")
set(CMAKE_CXX_FLAGS  "-Wno-deprecated-register ${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )
add_library(clas12 MODULE ${_clas12})
python_extension_module(clas12)
target_link_libraries(clas12 hipocpp4_static ${LZ4_LIBRARY} ${PYTHON_LIBRARIES} ${ROOT_LIBRARIES})
install(TARGETS clas12
        LIBRARY DESTINATION lib)
