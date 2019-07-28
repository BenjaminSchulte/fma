file(GLOB_RECURSE FMAJSON_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(fmajson SHARED ${FMAJSON_SRC})
set_target_properties(fmajson PROPERTIES VERSION 1.0.0)
set_target_properties(fmajson PROPERTIES SOVERSION 1)
target_include_directories(fmajson PRIVATE include)
target_include_directories(fmajson PRIVATE ../libfma/include)

configure_file(fmajson.pc.in fmajson.pc @ONLY)
install(TARGETS fmajson LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/fmajson/fmajson.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

