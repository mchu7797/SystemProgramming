//
// Created by Minseok Chu on 11/1/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASM_FILENAME "week9.asm"
#define ASM_LINE_LENGTH 80

struct macro_info {
    char keyword[15];
    char filename[20];
};

int parse_macro(char *asm_filename, struct macro_info **macro_table, int *macro_table_length) {
    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    char asm_data[ASM_LINE_LENGTH];
    long asm_pos = ftell(asm_file);

    int macro_mode = 0;
    FILE *macro_file;

    while (fgets(asm_data, ASM_LINE_LENGTH, asm_file) != NULL) {
        char *token = strtok(asm_data, " ,:\n\t");
        char *token_history = asm_data;

        while (token != NULL) {
            if (strncmp(token, "KMAC", 4) == 0) {
                char macro_keyword[20];
                strcpy(macro_keyword, token_history);

                strcpy((*macro_table)[*macro_table_length].keyword, macro_keyword);
                strcat(macro_keyword, ".TXT");
                strcpy((*macro_table)[*macro_table_length].filename, macro_keyword);

                macro_file = fopen((*macro_table)[*macro_table_length].filename, "w");
                macro_mode = 1;
            }

            if (strncmp(token, "ENDK", 4) == 0) {
                (*macro_table_length)++;
                fclose(macro_file);
                macro_mode = 0;
            }

            token_history = token;
            token = strtok(NULL, " ,:\n\t");
        }

        if (macro_mode > 1) {
            fseek(asm_file, asm_pos, SEEK_SET);
            int c;

            while ((c = fgetc(asm_file)) != '\n') {
                fputc(c, macro_file);
            }

            fputc('\n', macro_file);
        }

        if (macro_mode > 0) { macro_mode++; }

        asm_pos = ftell(asm_file);
    }

    if (!feof(asm_file)) {
        return 1;
    }

    fclose(asm_file);

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
    // 3 => Macro block starts
    // 2 => Macro block ends
    // 1 => Macro call
    // 0 => Isn't macro
    int macro_flag = 0;

    char asm_data[ASM_LINE_LENGTH] = {0};
    long asm_file_offset = ftell(asm_file);

    while (fgets(asm_data, ASM_LINE_LENGTH, asm_file) != NULL) {
        char *token;

        token = strtok(asm_data, " \n\t");

        while (token != NULL) {
            if (strncmp(token, "KMAC", 4) == 0) {
                macro_flag = 3;
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

        if (macro_flag < 3) {
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
    struct macro_info *macro_table = (struct macro_info *) malloc(sizeof(struct macro_info) * 10);
    int macro_table_length = 0;

    if (parse_macro(ASM_FILENAME, &macro_table, &macro_table_length)) {
        // If execution does not work properly
        puts("Execution Failed : PARSING MACRO");
        return 1;
    }

    if (print_assembly(ASM_FILENAME, macro_table, macro_table_length)) {
        // If execution does not work properly
        puts("Execution Failed : PRINTING ASSEMBLY");
        return 1;
    }

    free(macro_table);

    return 0;
}