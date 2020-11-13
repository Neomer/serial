cmake_minimum_required(VERSION 3.17)
project(serial)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_STANDARD 14)

add_compile_options(-pthread)
add_executable(serial main.cpp Serial.cpp Serial.h)

