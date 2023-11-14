//
// Created by Minseok Chu on 11/13/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keyword_table.h"

struct variable_info {
    // 8 bits or 16 bits. (DB, DW)
    int data_kinds;
    char *data_binary;
};

struct keywords_info {
    char keyword[10];
    int memory_offsets;
    int is_variable;
    struct variable_info *variableInfo;
};

int extract_data_symbols(char *filename, struct keywords_info **data_symbols, int *data_symbols_length) {
    FILE *assembly_file = fopen(filename, "r");
    if (assembly_file == NULL) {
        puts("Error: Cannot open assembly file!");
        return 1;
    }

    if (*data_symbols == NULL) {
        *data_symbols = (char *)malloc(sizeof(struct keywords_info) * 15);
        *data_symbols_length = 15;
    }

    long assembly_data_length;
    char* assembly_data;

    fseek(assembly_file, 0, SEEK_END);
    assembly_data_length = ftell(assembly_file);
    fseek(assembly_file, 0, SEEK_SET);

    assembly_data = (char *)malloc(assembly_data_length);
    fread(assembly_data, 1, assembly_data_length, assembly_file);

    char* keyword_token = strtok(assembly_data, " :,\r\n\t");

    while (keyword_token != NULL) {
        for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
            if (strncmp(keyword_token, command_list[i], KEYWORD_LENGTH) == 0) {
                keyword_token = strtok(NULL, " :,\r\n\t");
                continue;
            }
        }

        for (int i = 0; i < BYTE_REGISTER_LIST_LENGTH; i++) {
            if (strncmp(keyword_token, byte_register_list[i], KEYWORD_LENGTH) == 0) {
                keyword_token = strtok(NULL, " :,\r\n\t");
                continue;
            }
        }

        for (int i = 0; i < WORD_REGISTER_LIST_LENGTH; i++) {
            if (strncmp(keyword_token, word_register_list[i], KEYWORD_LENGTH) == 0) {
                keyword_token = strtok(NULL, " :,\r\n\t");
                continue;
            }
        }

        for (int i = 0; i < DIRECTION_LIST_LENGTH; i++) {
            if (strncmp(keyword_token, direction_list[i], KEYWORD_LENGTH) == 0) {
                keyword_token = strtok(NULL, " :,\r\n\t");
                continue;
            }
        }
    }



    free(assembly_data);
}

int main() {


    return 0;
}