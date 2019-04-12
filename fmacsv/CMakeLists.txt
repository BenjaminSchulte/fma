file(GLOB_RECURSE FMACSV_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmacsv ${FMACSV_SRC})
target_include_directories(fmacsv PRIVATE include)
target_include_directories(fmacsv PRIVATE ../libfma/include)
