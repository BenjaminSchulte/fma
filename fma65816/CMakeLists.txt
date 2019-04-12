file(GLOB_RECURSE FMA65816_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fma65816 ${FMA65816_SRC})
target_include_directories(fma65816 PRIVATE include)
target_include_directories(fma65816 PRIVATE ../libfma/include)