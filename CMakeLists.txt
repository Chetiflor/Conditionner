#CMakeLists.txt to build project using cmake
#run $cmake CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(pgm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("projet/src/")

file(
GLOB_RECURSE
source_files
projet/src/*.[cht]pp
)

set(CMAKE_BUILD_TYPE Debug)

if (WIN32)
  set(LINK_FLAGS )
elseif (APPLE)
  set(LINK_FLAGS )
elseif (UNIX)
  set(LINK_FLAGS -lm -ldl -fopenmp)
  add_definitions(-O2 -Wno-comment -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -DCORRECTION -DWITH_BACKTRACE)
endif ()

add_executable(pgm ${source_files})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(pgm PUBLIC OpenMP::OpenMP_CXX ${LINK_FLAGS})
else()
    target_link_libraries(pgm ${LINK_FLAGS} )
endif()
