cmake_minimum_required(VERSION 3.7.2)
project(fma VERSION 1.0.0 DESCRIPTION "famicom macro assembler")

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wunused-result -Winit-self -Wno-strict-aliasing -Wcast-align -Wcast-qual -Wuninitialized -Wmissing-declarations -Wnon-virtual-dtor -Werror")

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release")
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if (${DEBUG_FMA})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_FMA")
endif()

add_subdirectory(libfma)
add_subdirectory(fma65816)
add_subdirectory(fmaspc)
add_subdirectory(fmamemory)
add_subdirectory(fmacsv)
add_subdirectory(fmafs)
add_subdirectory(fmajson)
add_subdirectory(fmaimage)

if (CMAKE_JS_VERSION)
  add_subdirectory(fmajs)
else()
  add_subdirectory(fma)
endif()

enable_testing()
add_subdirectory(tests)
