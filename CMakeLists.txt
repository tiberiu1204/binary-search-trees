cmake_minimum_required(VERSION 3.27)
project(binary_search_trees)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_executable(binary_search_trees main.cpp
        include/bst.h
        include/scapegoat.h
        include/avl.h
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
