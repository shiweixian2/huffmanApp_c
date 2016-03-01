cmake_minimum_required(VERSION 3.3)
project(huffmanApp_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp CreateCode.cpp fortest.cpp fortest.h)
add_executable(huffmanApp_c ${SOURCE_FILES})