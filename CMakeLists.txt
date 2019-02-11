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
set(CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")

# Find the lz4 package
# Hopefully in the future if not found download and build
find_package(LZ4 REQUIRED)
add_definitions(-D__LZ4__)

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
add_subdirectory(src/recon2root)
add_subdirectory(src/dst2root)
