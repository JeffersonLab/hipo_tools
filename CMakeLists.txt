cmake_minimum_required(VERSION 3.5)
project(hipoio)
set(VERSION 0.0.1)

# Add modules to find find packages
set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake)

# Add root support
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "-Ofast ${ROOT_CXX_FLAGS}")

# Find the lz4 package
# Hopefully in the future if not found download and build
find_package(LZ4)
IF(LZ4_FOUND)
  add_definitions(-D__LZ4__)
  include_directories(${LZ4_INCLUDE_DIR})
ELSE()
  add_subdirectory(src/lz4/contrib/cmake_unofficial)
  set(LZ4_LIBRARY lz4_static)
  set(LZ4_INCLUDE_DIR src/lz4/lib)
  include_directories(${LZ4_INCLUDE_DIR})
  add_definitions(-D__LZ4__)
ENDIF()

# Add directory for hipo3 library
include_directories(src/hipocpp3/include)
add_subdirectory(src/hipocpp3)

# Add directory for hipo4 library
include_directories(src/hipocpp4/include)
add_subdirectory(src/hipocpp4)

# If cython found also build hipopy3/4
find_package(Cython)
if(Cython_FOUND)
  add_subdirectory(src/hipopy)
endif()

# Build executables from their own folders
add_subdirectory(src/hipo2root)
add_subdirectory(src/dst2root)

# Build examples
add_subdirectory(examples)


# -------------------------
# install library config
include(CMakePackageConfigHelpers)

# Install and export targets
#install(EXPORT HipoToolsTargets
#    FILE HipoToolsTargets.cmake
#    NAMESPACE HipoTools::
#    DESTINATION lib/hipo
#    )


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
