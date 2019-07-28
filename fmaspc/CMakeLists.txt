file(GLOB_RECURSE FMASPC_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmaspc SHARED ${FMASPC_SRC})
set_target_properties(fmaspc PROPERTIES VERSION 1.0.0)
set_target_properties(fmaspc PROPERTIES SOVERSION 1)
target_include_directories(fmaspc PRIVATE include)
target_include_directories(fmaspc PRIVATE ../libfma/include)

configure_file(fmaspc.pc.in fmaspc.pc @ONLY)
install(TARGETS fmaspc LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fmaspc/fmaspc.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

