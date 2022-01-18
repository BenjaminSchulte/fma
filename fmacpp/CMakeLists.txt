file(GLOB_RECURSE FMACPP_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmacpp SHARED ${FMACPP_SRC})
set_target_properties(fmacpp PROPERTIES VERSION 1.0.0)
set_target_properties(fmacpp PROPERTIES SOVERSION 1)
target_include_directories(fmacpp PRIVATE include)
target_include_directories(fmacpp PRIVATE ../libfma/include)

configure_file(fmacpp.pc.in fmacpp.pc @ONLY)
install(TARGETS fmacpp LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fmacpp/fmacpp.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
