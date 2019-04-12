file(GLOB_RECURSE FMAFS_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmafs ${FMAFS_SRC})
target_include_directories(fmafs PRIVATE include)
target_include_directories(fmafs PRIVATE ../libfma/include)
