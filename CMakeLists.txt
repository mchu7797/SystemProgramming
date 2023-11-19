cmake_minimum_required(VERSION 3.26)
project(SystemProgramming C)

set(CMAKE_C_STANDARD 11)

add_executable(Week1 c-code/week1.c)
add_executable(Week2 c-code/week2.c)
add_executable(Week3 c-code/week3.c)
add_executable(Week4 c-code/week4.c)
add_executable(Week5 c-code/week5.c)
add_executable(Week6 c-code/week6.c)
add_executable(MiddleExam8 c-code/middle-exam-8.c)
add_executable(Week9 c-code/week9.c)
add_executable(Week10 c-code/week10.c
        c-code/week10.h)

# for execute week2
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/fruits.txt ${CMAKE_CURRENT_BINARY_DIR}/fruits.txt COPYONLY)

# For execute week3 ~ week5
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/d.txt ${CMAKE_CURRENT_BINARY_DIR}/d.txt COPYONLY)

# For execute week6
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/input.asm ${CMAKE_CURRENT_BINARY_DIR}/input.asm COPYONLY)

# For execute week9
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/week9.asm ${CMAKE_CURRENT_BINARY_DIR}/week9.asm COPYONLY)

# For execute week10
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/week10.asm ${CMAKE_CURRENT_BINARY_DIR}/week10.asm COPYONLY)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/instruction_list.txt ${CMAKE_CURRENT_BINARY_DIR}/instruction_list.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/inputs/register_list.txt ${CMAKE_CURRENT_BINARY_DIR}/register_list.txt COPYONLY)
add_executable(Assembler assembler/assembler.h assembler/assembler.c)