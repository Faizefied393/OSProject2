cmake_minimum_required(VERSION 3.20)
project(cyb3053_project2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

include_directories(include)
set(SOURCES src/alloc.c src/test.c)

add_executable(cyb3053_project2 ${SOURCES})
