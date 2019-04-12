file(GLOB_RECURSE FMAMEMORY_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmamemory ${FMAMEMORY_SRC})
target_include_directories(fmamemory PRIVATE include)
target_include_directories(fmamemory PRIVATE ../libfma/include)