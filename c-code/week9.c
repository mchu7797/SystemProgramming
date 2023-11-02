//
// Created by Minseok Chu on 11/1/23.
// Work in Progress
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASM_FILENAME "input.asm"



int parse_macro(char *asm_filename, char) {
    int macro_list_length = 10;
    macro_list = init_macro_list(macro_list_length);

    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    fclose(asm_file);
    return 0;
}

int parse_assembly(char *asm_filename, struct macro_data macro_list[]) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    fclose(asm_file);
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