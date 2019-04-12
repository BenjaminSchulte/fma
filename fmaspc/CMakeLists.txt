file(GLOB_RECURSE FMASPC_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmaspc ${FMASPC_SRC})
target_include_directories(fmaspc PRIVATE include)
target_include_directories(fmaspc PRIVATE ../libfma/include)