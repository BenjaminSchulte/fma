file(GLOB_RECURSE FMAIMAGE_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if(NOT DEFINED WITH_FMAIMAGE)
  set(WITH_FMAIMAGE true CACHE STRING "Include FMA image")
endif()
find_package(PNG)

if (PNG_FOUND)
  add_library(fmaimage ${FMAIMAGE_SRC})
  target_include_directories(fmaimage PRIVATE include)
  target_include_directories(fmaimage PRIVATE ${PNG_INCLUDE_DIRS})
  target_include_directories(fmaimage PRIVATE ../libfma/include)
  target_link_libraries(fmaimage ${PNG_LIBRARIES})
else()
  set(WITH_FMAIMAGE false)
endif()
