//
// Created by Minseok Chu on 11/1/23.
// Work in Progress
//

#include <stdio.h>
#include <string.h>

#define ASM_FILENAME "input.asm"

struct macro_data {
    char keyword[10];
    char data_file_path[20];
};

int parse_macro(char *asm_filename, struct macro_data macro_list[]) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    return 0;
}

int parse_assembly(char *asm_filename, struct macro_data macro_list[]) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    return 0;
}

int main(void) {
    struct macro_data *macro_list;

    if (parse_macro(ASM_FILENAME, macro_list)) {
        // If execution does not work properly
        return 1;
    }

    if (parse_assembly(ASM_FILENAME, macro_list)) {
        // If execution does not work properly
        return 1;
    }

    return 0;
}