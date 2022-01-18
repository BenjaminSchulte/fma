file(GLOB_RECURSE FMA_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if(WITH_FMAIMAGE)
  add_definitions(-DWITH_FMAIMAGE)
endif()

add_executable(fma ${FMA_SRC})
target_include_directories(fma PUBLIC include)
target_link_libraries(fma fma65816 fmaspc fmamemory fmacsv fmajson fmafs fmasuperfx fmacpp)

if(WITH_FMAIMAGE)
  target_link_libraries(fma fmaimage)
endif()

target_link_libraries(fma libfma)
install(TARGETS fma RUNTIME DESTINATION bin)
