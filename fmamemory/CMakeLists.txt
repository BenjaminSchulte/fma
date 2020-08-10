file(GLOB_RECURSE FMAMEMORY_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmamemory SHARED ${FMAMEMORY_SRC})
set_target_properties(fmamemory PROPERTIES VERSION 1.0.0)
set_target_properties(fmamemory PROPERTIES SOVERSION 1)
target_include_directories(fmamemory PRIVATE include)
target_include_directories(fmamemory PRIVATE ../libfma/include)

configure_file(fmamemory.pc.in fmamemory.pc @ONLY)
install(TARGETS fmamemory LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/memory DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}fma/)
install(FILES ${CMAKE_BINARY_DIR}/fmamemory/fmamemory.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

