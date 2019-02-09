cmake_minimum_required(VERSION 3.5)
project(hipoio)
set(VERSION 0.0.1)

set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake
   ${PROJECT_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXTRA_FLAGS "-D__LZ4__ -pthread -m64")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" )

## OS Specific settings
include(cmake/os.cmake)

find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED COMPONENTS Tree)
include(${ROOT_USE_FILE})
## extra root settings
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})
include(cmake/root.cmake)

include_directories(src/hipocpp3/include)
include_directories(src/hipocpp4/include)

add_subdirectory(src/hipocpp3)
add_subdirectory(src/hipocpp4)
add_subdirectory(src/recon2root)

if(NOT APPLE)
  add_subdirectory(src/toohip4root)
  add_subdirectory(src/hipo-io)
endif()


find_package(Cython)
if(Cython_FOUND)
    add_subdirectory(src/hipopy)
endif()


# -------------------------
# install library config
include(CMakePackageConfigHelpers)

# Install and export targets
install(EXPORT HipoToolsTargets
    FILE HipoToolsTargets.cmake
    NAMESPACE HipoTools::
    DESTINATION lib/hipo
    )


set(TARGETS_INSTALL_PATH lib/hipo/HipoToolsTargets.cmake)
CONFIGURE_PACKAGE_CONFIG_FILE(
  cmake/HipoToolsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/HipoToolsConfig.cmake
    INSTALL_DESTINATION lib/hipo
    PATH_VARS TARGETS_INSTALL_PATH
    )

  write_basic_package_version_file("HipoToolsConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
    )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/HipoToolsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/HipoToolsConfigVersion.cmake
  DESTINATION lib/hipo
  )
