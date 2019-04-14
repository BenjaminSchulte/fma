file(GLOB_RECURSE LIBFMA_SRC src *.cpp)

if (CMAKE_JS_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
else()
  set(Boost_USE_STATIC_LIBS ON)
endif()


set(CMAKE_CXX_STANDARD 11)

set(ICU_DEBUG ON)

add_definitions(-DFMA_VERSION=\"0.9.0\")

find_package(Boost REQUIRED COMPONENTS program_options filesystem locale thread)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

bison_target(FmaParser grammar/fma.y ${CMAKE_CURRENT_BINARY_DIR}/fma.y.cpp VERBOSE)
flex_target(FmaScanner grammar/fma.l ${CMAKE_CURRENT_BINARY_DIR}/fma.l.cpp)
add_flex_bison_dependency(FmaScanner FmaParser)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/fma.l.cpp PROPERTIES COMPILE_FLAGS -Wno-sign-compare)

add_library(libfma ${LIBFMA_SRC} ${BISON_FmaParser_OUTPUTS} ${FLEX_FmaScanner_OUTPUTS})
target_include_directories(libfma PRIVATE ${Boost_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
target_include_directories(libfma PUBLIC include)
target_link_libraries(libfma ${Boost_LIBRARIES} ${FLEX_LIBRARIES} ${ICU_LIBRARIES})
