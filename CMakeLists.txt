project(CPP_project_mindb CXX)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(home /home/lqx/code/HKUSTGZ-24UG-UFUG2601-CPP-Project)
# set(EXECUTABLE_OUTPUT_PATH ${home}/build)
set(LIBRARY_OUTPUT_PATH ${home}/lib)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
link_directories(${home}/lib)
include_directories(${home})

set(PART1 ${home}/minSQL_struct.hpp ${home}/minSQL_process.cpp ${home}/minSQL_struct.cpp)
set(CAL ${home}/calculator.cpp)
# set(LIBRARY_OUTPUT_PATH ${home}/lib)
add_library(core SHARED ${PART1})
add_library(calculator SHARED ${CAL})
# message(${PART1})
add_executable(a ${home}/main.cpp)
target_link_libraries(a core)
target_link_libraries(a calculator)
