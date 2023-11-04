//
// Created by Minseok Chu on 11/1/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASM_FILENAME "input.asm"

struct macro_info {
    char keyword[15];
    char filename[20];
};

int parse_macro(char *asm_filename, struct macro_info **macro_table, int *macro_table_length) {
    *macro_table = (struct macro_info *) malloc(sizeof(struct macro_info) * 10);
    *macro_table_length = 0;

    FILE *asm_file = fopen(asm_filename, "r");

    if (asm_file == NULL) {
        return 1;
    }

    char asm_data[35];
    long asm_pos = ftell(asm_file);

    int macro_mode = 0;
    FILE *macro_file;

    while (fgets(asm_data, 35, asm_file) != NULL) {
        char *token = strtok(asm_data, " ,:\n");
        char *token_history = asm_data;

        while (token != NULL) {
            if (strncmp(token, "MACRO", 5) == 0) {
                char macro_keyword[20];
                strcpy(macro_keyword, token_history);

                strcpy((*macro_table)[*macro_table_length].keyword, macro_keyword);
                strcat(macro_keyword, ".TXT");
                strcpy((*macro_table)[*macro_table_length].filename, macro_keyword);

                macro_file = fopen((*macro_table)[*macro_table_length].filename, "w");
                macro_mode = 1;
            }

            if (strncmp(token, "ENDM", 4) == 0) {
                if (strcmp(token_history, (*macro_table)[*macro_table_length].keyword) == 0) {
                    (*macro_table_length)++;
                    fclose(macro_file);
                    macro_mode = 0;
                }
            }

            token_history = token;
            token = strtok(NULL, " ,:\n");
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

    // It detects macro call or macro block ends.
    int macro_flag = 0;
    int macro_block_detected = 0;

    char asm_data[35];
    long asm_file_offset = ftell(asm_file);

    while (fgets(asm_data, 35, asm_file) != NULL) {
        char *token = strtok(asm_data, " ,:\n");

        while (token != NULL) {
            if (strncmp(token, "MACRO", 5) == 0) {
                macro_block_detected = 1;
            }

            if (strncmp(token, "ENDM", 4) == 0) {
                macro_block_detected = 0;
                macro_flag = 1;
            }

            token = strtok(NULL, " ,:\n");
        }

        token = strtok(asm_data, " ,:\n");

        while (token != NULL && !macro_block_detected && !macro_flag) {
            for (int i = 0; i < macro_table_length; i++) {
                if (strcmp(macro_table[i].keyword, token) != 0) {
                    continue;
                }

                if (print_macro(macro_table[i].filename)) {
                    return 1;
                } else {
                    macro_flag = 1;
                }
            }

            token = strtok(NULL, " ,:\n");
        }

        if (!(macro_flag || macro_block_detected)) {
            fseek(asm_file, asm_file_offset, SEEK_SET);
            int c;

            while ((c = fgetc(asm_file)) != '\n') { putchar(c); }
            putchar('\n');
        }

        macro_flag = macro_flag == 1 ? 0 : macro_flag;
        asm_file_offset = ftell(asm_file);
    }

    if (!feof(asm_file)) {
        return 1;
    }

    fclose(asm_file);
    return 0;
}

int main(void) {
    struct macro_info *macro_table;
    int macro_table_length;

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