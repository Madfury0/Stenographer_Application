cmake_minimum_required(VERSION 3.10)
project(StegHide_C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_executable(steg_c src/steg.c)
