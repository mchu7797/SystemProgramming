//
// Created by Minseok Chu on 11/1/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASM_FILENAME "week9.asm"
#define ASM_LINE_LENGTH 80

struct macro_info {
    char keyword[20];
    char filename[20];
};

int parse_macro(char *asm_filename, struct macro_info **macro_table, int *macro_table_length) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    FILE *macro_file;

    char asm_data[ASM_LINE_LENGTH];
    long asm_file_offset = 0;

    // It represents macro block starts and ends
    // 1 => Macro block starts (Parsing and writing start)
    // 2 => Macro block ends (Parsing and writing end)
    int macro_mode = 0;
    int macro_table_index = 0;

    while (fgets(asm_data, ASM_LINE_LENGTH, asm_file) != NULL) {
        char *token = strtok(asm_data, " ,:\n\t");

        // For find macro name.
        // Macro name always exists "KMAC" keyword before.
        char *token_history = asm_data;

        while (token != NULL) {
            if (strncmp(token, "KMAC", 4) == 0) {
                if (macro_table_index >= *macro_table_length) {
                    return 1;
                }

                strcpy((*macro_table)[macro_table_index].keyword, token_history);
                strcpy((*macro_table)[macro_table_index].filename, token_history);
                strcat((*macro_table)[macro_table_index].filename, ".TXT");

                macro_file = fopen((*macro_table)[macro_table_index].filename, "w");

                if (macro_file == NULL) {
                    return 1;
                }

                macro_mode = 1;
            }

            if (strncmp(token, "ENDK", 4) == 0) {
                macro_table_index++;
                fclose(macro_file);
                macro_mode = 0;
            }

            token_history = token;
            token = strtok(NULL, " ,:\n\t");
        }

        if (macro_mode > 1) {
            fseek(asm_file, asm_file_offset, SEEK_SET);
            int c;

            while ((c = fgetc(asm_file)) != '\n') {
                fputc(c, macro_file);
            }

            fputc('\n', macro_file);
        }

        if (macro_mode > 0) { macro_mode++; }

        asm_file_offset = ftell(asm_file);
    }

    if (!feof(asm_file)) {
        return 1;
    }

    fclose(asm_file);

    *macro_table_length = macro_table_index;

    return 0;
}

int print_macro(char *macro_filename) {
    FILE *macro_file = fopen(macro_filename, "r");

    if (macro_file == NULL) {
        return 1;
    }

    int c;

    while ((c = getc(macro_file)) != EOF) {
        putchar(c);
    }

    fclose(macro_file);

    return 0;
}

int print_assembly(char *asm_filename, struct macro_info *macro_table, int macro_table_length) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    // It represents detected macro calls or macro block
    // 2 => Macro block starts
    // 1 => Macro block ends
    // 0 => Isn't macro
    int macro_flag = 0;

    char asm_data[ASM_LINE_LENGTH] = {0};
    long asm_file_offset = ftell(asm_file);

    while (fgets(asm_data, ASM_LINE_LENGTH, asm_file) != NULL) {
        char *token;

        token = strtok(asm_data, " \n\t");

        while (token != NULL) {
            if (strncmp(token, "KMAC", 4) == 0) {
                macro_flag = 2;
            }

            if (strncmp(token, "ENDK", 4) == 0) {
                macro_flag = 1;
            }

            token = strtok(NULL, " \n\t");
        }

        // Needs to re-read line because strtok() modifies delimiter char to null char
        // Example :
        // "Hello, world" -> strtok(STR, " ") -> "Hello,\0world"
        fseek(asm_file, asm_file_offset, SEEK_SET);
        fgets(asm_data, ASM_LINE_LENGTH, asm_file);

        token = strtok(asm_data, " \n\t");

        while (token != NULL && !macro_flag) {
            for (int i = 0; i < macro_table_length; i++) {
                if (strcmp(token, macro_table[i].keyword) == 0) {
                    if (print_macro(macro_table[i].filename)) {
                        return 1;
                    } else {
                        macro_flag = 1;
                    }
                }
            }

            token = strtok(NULL, " \n\t");
        }

        if (!macro_flag) {
            fseek(asm_file, asm_file_offset, SEEK_SET);
            int c;

            for (int i = 0; i < ASM_LINE_LENGTH; i++) {
                c = fgetc(asm_file);

                if (c == '\n' || c == EOF) {
                    break;
                }

                putchar(c);
            }

            putchar('\n');
        }

        if (macro_flag < 2) {
            macro_flag = 0;
        }

        asm_file_offset = ftell(asm_file);
    }

    // When cannot read properly
    if (!feof(asm_file)) {
        return 1;
    }

    fclose(asm_file);
    return 0;
}

int main(void) {
    int macro_table_length = 10;
    struct macro_info *macro_table = (struct macro_info *) malloc(sizeof(struct macro_info) * macro_table_length);

    // PASS 1
    if (parse_macro(ASM_FILENAME, &macro_table, &macro_table_length)) {
        // If execution does not work properly
        puts("Execution Failed : PARSING MACRO");
        return 1;
    }

    // PASS 2
    if (print_assembly(ASM_FILENAME, macro_table, macro_table_length)) {
        // If execution does not work properly
        puts("Execution Failed : PRINTING ASSEMBLY");
        return 1;
    }

    free(macro_table);

    return 0;
}