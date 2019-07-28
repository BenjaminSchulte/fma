file(GLOB_RECURSE FMAIMAGE_SRC src *.cpp)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

if(NOT DEFINED WITH_FMAIMAGE)
  set(WITH_FMAIMAGE true CACHE STRING "Include FMA image")
endif()
find_package(PNG)

if (PNG_FOUND)
  add_library(fmaimage SHARED ${FMAIMAGE_SRC})
  set_target_properties(fmaimage PROPERTIES VERSION 1.0.0)
  set_target_properties(fmaimage PROPERTIES SOVERSION 1)
  target_include_directories(fmaimage PRIVATE include)
  target_include_directories(fmaimage PRIVATE ${PNG_INCLUDE_DIRS})
  target_include_directories(fmaimage PRIVATE ../libfma/include)
  target_link_libraries(fmaimage ${PNG_LIBRARIES})

  configure_file(fmaimage.pc.in fmaimage.pc @ONLY)
  install(TARGETS fmaimage LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES ${CMAKE_BINARY_DIR}/fmaimage/fmaimage.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
else()
  set(WITH_FMAIMAGE false)
endif()
