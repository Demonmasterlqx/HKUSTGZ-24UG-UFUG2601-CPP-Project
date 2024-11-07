project(CPP_project_mindb CXX)
cmake_minimum_required(VERSION 3.5)

set(home /home/lqx/code/C++_project_mindb/)
set(LIBRARY_OUTPUT_DIRECTORY ${home}/lib)
set(EXECUTABLE_OUTPUT_PATH ${home}/build)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB src ${home}/*cpp ${home}/*hpp)
message(${src})
add_executable(a ${src})