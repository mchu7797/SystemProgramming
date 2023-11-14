//
// Created by Minseok Chu on 2023-11-08.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "week10.h"

#define ASM_FILENAME "week10.asm"

int
extract_data_symbol_keywords(char* filename, char* data_symbol_list, int* data_symbol_list_length) {
    FILE* asm_file = fopen(filename, "r");

    if (asm_file == NULL) {
        puts("ERROR: Cannot open assembly file!");
        return 1;
    }

    char asm_data[80];
    int data_symbol_list_index = 0;

    while (fgets(asm_data, 80, asm_file) != NULL) {
        char* token = strtok(asm_data, " :,\n\t");
        char* token_before = NULL;

        while (token != NULL) {
            for (int i = 0; i < DATA_DEFINE_LIST_LENGTH; i++) {
                if (strncmp(data_define_list[i], token, KEYWORD_LENGTH) == 0) {
                    if (token_before == NULL) {
                        puts("ERROR: Invalid assembly code!");
                        return 1;
                    }
                    if (*data_symbol_list_length <= data_symbol_list_index) {
                        puts("ERROR: Too many data symbol!");
                        return 1;
                    }

                    strcpy(data_symbol_list + (KEYWORD_LENGTH * data_symbol_list_index), token_before);
                    data_symbol_list_index++;

                    break;
                }
            }

            token_before = token;
            token = strtok(NULL, " :,\n\t");
        }
    }

    *data_symbol_list_length = data_symbol_list_index;

    return 0;
}

int
print_assembly_code(char* filename, char* data_symbol_list, int data_symbol_list_length) {
    FILE* asm_file = fopen(filename, "r");

    if (asm_file == NULL) {
        puts("ERROR: Cannot open assembly file!");
        return 1;
    }

    char asm_data[80];

    while (fgets(asm_data, 80, asm_file) != NULL) {
        char asm_data_for_print[80];
        strncpy(asm_data_for_print, asm_data, 80);

        char* print_token = strtok(asm_data_for_print, " \t\n");

        while (print_token != NULL) {
            printf("%s ", print_token);
            print_token = strtok(NULL, " \t\n");
        }

        printf("\n");

        char* token = strtok(asm_data, " :,\n\t");

        while (token != NULL) {
            int command_found = 0;

            for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
                if (strncmp(command_list[i], token, KEYWORD_LENGTH) == 0) {
                    printf("OPCD ");
                    command_found = 1;
                }
            }

            for (int i = 0; i < BYTE_REGISTER_LIST_LENGTH; i++) {
                if (strncmp(byte_register_list[i], token, KEYWORD_LENGTH) == 0) {
                    printf("REG08 ");
                    command_found = 1;
                }
            }

            for (int i = 0; i < WORD_REGISTER_LIST_LENGTH; i++) {
                if (strncmp(word_register_list[i], token, KEYWORD_LENGTH) == 0) {
                    printf("REG16 ");
                    command_found = 1;
                }
            }

            for (int i = 0; i < DIRECTION_LIST_LENGTH; i++) {
                if (strncmp(direction_list[i], token, KEYWORD_LENGTH) == 0) {
                    printf("DIREC ");
                    command_found = 1;
                }
            }

            for (int i = 0; i < data_symbol_list_length; i++) {
                if (strncmp(data_symbol_list + (i * KEYWORD_LENGTH), token, KEYWORD_LENGTH) == 0) {
                    printf("SYMB ");
                    command_found = 1;
                }
            }

            if (atoi(token) != 0) {
                printf("NUMB ");
                command_found = 1;
            }

            if (!command_found) {
                printf("LABL ");
            }

            token = strtok(NULL, " :,\n\t");
        }

        printf("(DETECTED)\n");
    }

    return 0;
}

int
main() {
    int data_symbol_list_length = 10;
    char* data_symbol_list = (char*)malloc(data_symbol_list_length);

    if (extract_data_symbol_keywords(ASM_FILENAME, data_symbol_list, &data_symbol_list_length)) {
        puts("Program exited cause of above error!");
        return 1;
    }

    if (print_assembly_code(ASM_FILENAME, data_symbol_list, data_symbol_list_length)) {
        puts("Program exited cause of above error!");
        return 1;
    }

    return 0;
}