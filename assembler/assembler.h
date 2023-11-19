//
// Created by minse on 2023-11-17.
//

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>

typedef struct {
    char name[3];
    char word_type[2];
    char number[4];
} register_t;

typedef struct {
    char instruction[6];
    char destination_memory[2];
    char source_memory[2];
    char word_type[2];
    char instruction_code[3];
    char instruction_code_length[2];
    char instruction_code_fixed[2];
    char mod_reg[9];
} instruction_t;

typedef struct {
    char name[10];
    char word_type[2];
    int32_t binary_offset;
    char data[10];
} symbol_t;

typedef struct {
    char label[10];
    char operator[10];
    char operand[3][10];
} asm_code_t;

#endif //ASSEMBLER_H
