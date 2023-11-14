//
// Created by Minseok Chu on 11/9/23.
//

#ifndef SYSTEMPROGRAMMING_WEEK10_H
#define SYSTEMPROGRAMMING_WEEK10_H

#define KEYWORD_LENGTH 10
#define COMMAND_LIST_LENGTH 12
#define BYTE_REGISTER_LIST_LENGTH 8
#define WORD_REGISTER_LIST_LENGTH 6
#define DATA_DEFINE_LIST_LENGTH 2
#define DIRECTION_LIST_LENGTH 4

char command_list[COMMAND_LIST_LENGTH][KEYWORD_LENGTH] = {
        "MOV",
        "ADD", "SUB", "AND", "OR",
        "INC", "DEC",
        "CMP",
        "JMP", "JA", "JB", "JE"
};

char byte_register_list[BYTE_REGISTER_LIST_LENGTH][KEYWORD_LENGTH] = {
        "AH", "AL",
        "BH", "BL",
        "CH", "CL",
        "DH", "DL"
};

char word_register_list[WORD_REGISTER_LIST_LENGTH][KEYWORD_LENGTH] = {
        "AX", "BX", "CX", "DX",
        "SI", "DI"
};

char data_define_list[DATA_DEFINE_LIST_LENGTH][KEYWORD_LENGTH] = {
        "DW", "DB"
};

char direction_list[DIRECTION_LIST_LENGTH][KEYWORD_LENGTH] = {
        "ASSUME", "SEGMENT", "ENDS", "END"
};

#endif //SYSTEMPROGRAMMING_WEEK10_H