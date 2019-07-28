file(GLOB_RECURSE FMAFS_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmafs SHARED ${FMAFS_SRC})
set_target_properties(fmafs PROPERTIES VERSION 1.0.0)
set_target_properties(fmafs PROPERTIES SOVERSION 1)
target_include_directories(fmafs PRIVATE include)
target_include_directories(fmafs PRIVATE ../libfma/include)

configure_file(fmafs.pc.in fmafs.pc @ONLY)
install(TARGETS fmafs LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fmafs/fmafs.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
