cmake_minimum_required(VERSION 3.5)
project(dg370919_mh359520_mk371148)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(SOURCE_FILES
        tests.cc
    geometry.cc
    geometry.h)

add_executable(dg370919_mh359520_mk371148 ${SOURCE_FILES})