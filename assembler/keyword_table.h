//
// Created by Minseok Chu on 11/13/23.
//

#ifndef SYSTEMPROGRAMMING_KEYWORD_TABLE_H
#define SYSTEMPROGRAMMING_KEYWORD_TABLE_H

#define KEYWORD_LENGTH 10

#define COMMAND_LIST_LENGTH 12
#define BYTE_REGISTER_LIST_LENGTH 8
#define WORD_REGISTER_LIST_LENGTH 6
#define DATA_DEFINE_LIST_LENGTH 2
#define DIRECTION_LIST_LENGTH 4

// Instruction Commands
char command_list[COMMAND_LIST_LENGTH][KEYWORD_LENGTH] = {
        "MOV",
        "ADD", "SUB", "AND", "OR",
        "INC", "DEC",
        "CMP",
        "JMP", "JA", "JB", "JE"
};

// 8 Bit Registers
char byte_register_list[BYTE_REGISTER_LIST_LENGTH][KEYWORD_LENGTH] = {
        "AH", "AL",
        "BH", "BL",
        "CH", "CL",
        "DH", "DL"
};

// 16 Bit Registers
char word_register_list[WORD_REGISTER_LIST_LENGTH][KEYWORD_LENGTH] = {
        "AX", "BX", "CX", "DX",
        "SI", "DI"
};

// Data define keywords
char data_define_list[DATA_DEFINE_LIST_LENGTH][KEYWORD_LENGTH] = {
        "DW", "DB"
};

// Assembler only keywords
char direction_list[DIRECTION_LIST_LENGTH][KEYWORD_LENGTH] = {
        "ASSUME", "SEGMENT", "ENDS", "END"
};

#endif //SYSTEMPROGRAMMING_KEYWORD_TABLE_H
