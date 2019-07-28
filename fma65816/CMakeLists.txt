file(GLOB_RECURSE FMA65816_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fma65816 SHARED ${FMA65816_SRC})
set_target_properties(fma65816 PROPERTIES VERSION 1.0.0)
set_target_properties(fma65816 PROPERTIES SOVERSION 1)
target_include_directories(fma65816 PRIVATE include)
target_include_directories(fma65816 PRIVATE ../libfma/include)

configure_file(fma65816.pc.in fma65816.pc @ONLY)
install(TARGETS fma65816 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fma65816/fma65816.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

