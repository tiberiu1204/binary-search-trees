cmake_minimum_required(VERSION 3.27)
project(binary_search_trees)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_executable(binary_search_trees main.cpp
        src/bst.cpp
        include/bst.h
        include/scapegoat.h
        src/scapegoat.cpp
)
