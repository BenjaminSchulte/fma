file(GLOB_RECURSE FMAJSON_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmajson ${FMAJSON_SRC})
target_include_directories(fmajson PRIVATE include)
target_include_directories(fmajson PRIVATE ../libfma/include)