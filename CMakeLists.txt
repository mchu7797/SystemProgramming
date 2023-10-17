cmake_minimum_required(VERSION 3.26)
project(SystemProgramming C)

set(CMAKE_C_STANDARD 11)

add_executable(Week1 week1.c)
add_executable(Week2 week2.c)
add_executable(Week3 week3.c)
add_executable(Week4 week4.c)
add_executable(Week5 week5.c)
add_executable(Week6 week6.c)

# for execute week2
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/fruits.txt ${CMAKE_CURRENT_BINARY_DIR}/fruits.txt COPYONLY)

# For execute week3 ~ week5
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/d.txt ${CMAKE_CURRENT_BINARY_DIR}/d.txt COPYONLY)

# For execute week6
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/input.asm ${CMAKE_CURRENT_BINARY_DIR}/input.asm COPYONLY)