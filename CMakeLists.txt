cmake_minimum_required(VERSION 3.24)
project(C_Compiler C)

set(CMAKE_C_STANDARD 11)

add_executable(C_Compiler main.c datastructures/map/map.c datastructures/map/map.h datastructures/map/entry.c datastructures/map/entry.h datastructures/linkedlist/linkedlist.c datastructures/linkedlist/linkedlist.h)
