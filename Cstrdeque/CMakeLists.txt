cmake_minimum_required(VERSION 3.5)
project(az297973_mk371148)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

set(SOURCE_FILES strdeque.cc strdeque.h strdequeconst.cc strdequeconst.h strdeque_test1.c)
add_executable(az297973_mk371148 ${SOURCE_FILES})