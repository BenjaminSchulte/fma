file(GLOB_RECURSE FMACSV_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmacsv SHARED ${FMACSV_SRC})
set_target_properties(fmacsv PROPERTIES VERSION 1.0.0)
set_target_properties(fmacsv PROPERTIES SOVERSION 1)
target_include_directories(fmacsv PRIVATE include)
target_include_directories(fmacsv PRIVATE ../libfma/include)

configure_file(fmacsv.pc.in fmacsv.pc @ONLY)
install(TARGETS fmacsv LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fmacsv/fmacsv.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

