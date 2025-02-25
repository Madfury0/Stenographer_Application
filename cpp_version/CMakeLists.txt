cmake_minimum_required(VERSION 3.10)
project(StegHide_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(steg_cpp src/steg.cpp)
