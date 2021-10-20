file(GLOB_RECURSE FMASUPERFX_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmasuperfx SHARED ${FMASUPERFX_SRC})
set_target_properties(fmasuperfx PROPERTIES VERSION 1.0.0)
set_target_properties(fmasuperfx PROPERTIES SOVERSION 1)
target_include_directories(fmasuperfx PRIVATE include)
target_include_directories(fmasuperfx PRIVATE ../libfma/include)

configure_file(fmasuperfx.pc.in fmasuperfx.pc @ONLY)
install(TARGETS fmasuperfx LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/superfx DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fma)
install(FILES ${CMAKE_BINARY_DIR}/fmasuperfx/fmasuperfx.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

