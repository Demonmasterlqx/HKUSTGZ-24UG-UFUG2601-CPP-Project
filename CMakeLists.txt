project(CPP_project_mindb CXX)
cmake_minimum_required(VERSION 3.5)

set(home /home/lqx/code/C++_project_mindb)
set(LIBRARY_OUTPUT_DIRECTORY ${home}/lib)
set(EXECUTABLE_OUTPUT_PATH ${home}/build)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
link_directories(${home}/lib)
include_directories(${home})

set(PART1 ${home}/minSQL_process.cpp ${home}/minSQL_struct.cpp ${home}/minSQL_struct.hpp)

add_library(core SHARED ${PART1})
message(${PART1})
add_executable(a ${home}/main.cpp)
target_link_libraries(a ${home}/lib/PART1)
