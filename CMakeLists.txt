cmake_minimum_required(VERSION 3.22)

project(allocator)

add_executable(main
    Src/allocator.c 
    Src/main.c 
    Src/utils.c
)