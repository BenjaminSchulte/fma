file(GLOB_RECURSE FMAJS_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

add_library(fma SHARED ${FMAJS_SRC})
set_target_properties(fma PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(fma PRIVATE include)
target_include_directories(fma PRIVATE ../libfma/include)
target_include_directories(fma PRIVATE ../fma65816/include)
target_include_directories(fma PRIVATE ${CMAKE_JS_INC})
target_link_libraries(fma fma65816 fmacsv fmafs fmajson fmamemory libfma ${CMAKE_JS_LIB})
