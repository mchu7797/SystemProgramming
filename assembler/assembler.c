/*
 * Created by minse on 2023-11-17.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "assembler.h"

#define RAW_CODE_LENGTH 50

/* Command information tables */
register_t registers[50];
instruction_t instructions[100];
symbol_t symbols[100];

/* Command information table's current length */
int32_t registers_length;
int32_t instructions_length;
int32_t symbols_length;

errno_t
init() {
    FILE *instruction_file, *register_file;

    instruction_file = fopen("instruction_list.txt", "r");
    register_file = fopen("register_list.txt", "r");

    if (instruction_file == NULL || register_file == NULL) {
        puts("Cannot open assembly keyword database!");
        return 1;
    }

    int32_t i = 0;

    while (!feof(instruction_file)) {
        fscanf(instruction_file, "%6s%2s%2s%4s%3s%2s%9s\n", instructions[i].instruction,
               instructions[i].destination_memory, instructions[i].source_memory, instructions[i].word_type,
               instructions[i].instruction_code, instructions[i].instruction_code_length, instructions[i].mod_reg);
        ++i;
    }

    instructions_length = i;

    i = 0;

    while (!feof(register_file)) {
        fscanf(register_file, "%s%s%s\n", registers[i].name, registers[i].word_type, registers[i].number);
        ++i;
    }

    registers_length = i;

    fclose(instruction_file);
    fclose(register_file);

    return 0;
}

int32_t
binary_to_int(char *binary_text) {
    int32_t i, result;

    i = 0;
    result = 0;

    while (*(binary_text + i) != '\0') {
        if (*(binary_text + i) != '1') {
            result += (int32_t) pow(2.0f, (double) (strlen(binary_text + i) - 1));
        }

        ++i;
    }

    return result;
}

int32_t
analyze_operand(char *operand) {
    int32_t i;

    if (isdigit(operand[0])) {
        /* Is digit */
        return 0;
    }

    for (i = 0; i < registers_length; i++) {
        if (strncmp(operand, registers[i].name, 3) == 0) {
            if (strncmp(registers[i].word_type, "w", 1) == 0) {
                /* Is word register */
                return 1;
            }

            if (strncmp(registers[i].word_type, "b", 1) == 0) {
                /* Is byte register */
                return 2;
            }
        }
    }

    if (strcmp(operand, "") == 0) {
        /* Is blank */
        return 4;
    } else {
        /* Is memory */
        return 3;
    }
}

/* 어샘블리 라인을 한번 읽고, 해당 어샘블리의 데이터 구조 정보를 불러 오는 것 */
errno_t
parse_assembly(char *raw_code, instruction_t *instruction_info, asm_code_t *parsed_code) {
    /* Cut raw string to keywords */
    /* Assembly basic syntax: [Label] Mnemonic [Operands] [;Comments] */
    char *keywords[5], *token;
    char raw_code_copied[RAW_CODE_LENGTH];
    int32_t keywords_length, i, j, k;

    strcpy(raw_code_copied, raw_code);
    token = strtok(raw_code_copied, " ,\t\n");

    keywords_length = 0;

    while (token != NULL && keywords_length < 5) {
        keywords[keywords_length++] = token;
        token = strtok(NULL, " ,\t\n");
    }

    for (i = 0; i < instructions_length; ++i) {
        if (strncmp(keywords[0], instructions[i].instruction, 6) == 0) {
            strcpy(parsed_code->label, "\0");
            strcpy(parsed_code->operator, keywords[0]);
            break;
        }
    }

    j = 1;
    k = 0;

    if (i == instructions_length) {
        strcpy(parsed_code->label, keywords[0]);
        strcpy(parsed_code->operator, keywords[1]);
        ++j;
    }

    while (j < 5 && k < 3) {
        if (keywords[j] == NULL || strncmp(keywords[j], "", 1) == 0) {
            strcpy(parsed_code->operand[k], "\0");
        } else {
            strcpy(parsed_code->operand[k], keywords[j]);
        }

        ++j;
        ++k;
    }

    strcpy(instruction_info->instruction, parsed_code->operator);

    switch (analyze_operand(parsed_code->operand[0])) {
        case 0:
            strcpy(instruction_info->destination_memory, "i");
            break;
        case 1:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 2:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 3:
            strcpy(instruction_info->destination_memory, "m");
            break;
        default:
            strcpy(instruction_info->destination_memory, "n");
            break;
    }

    switch (analyze_operand(parsed_code->operand[1])) {
        case 0:
            strcpy(instruction_info->source_memory, "i");
            break;
        case 1:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 2:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 3:
            strcpy(instruction_info->source_memory, "m");
            break;
        default:
            strcpy(instruction_info->source_memory, "n");
            break;
    }

    if (strcmp(instruction_info->destination_memory, "n") == 0 && strcmp(instruction_info->source_memory, "n") == 0) {
        strcpy(instruction_info->word_type, "n");
    }

    for (i = 0; i < instructions_length; ++i) {
        int32_t compare_flag = 0;

        if (strncmp(instructions[i].instruction, instruction_info->instruction, 6) == 0) {
            ++compare_flag;
        }

        if (strncmp(instructions[i].destination_memory, instruction_info->destination_memory, 2) == 0) {
            ++compare_flag;
        }

        if (strncmp(instructions[i].source_memory, instruction_info->source_memory, 2) == 0) {
            ++compare_flag;
        }

        if (strncmp(instructions[i].word_type, instruction_info->word_type, 2) == 0) {
            ++compare_flag;
        }

        if (compare_flag == 4) {
            strcpy(instruction_info->instruction_code, instructions[i].instruction_code);
            strcpy(instruction_info->instruction_code_length, instructions[i].instruction_code_length);
            strcpy(instruction_info->mod_reg, instructions[i].mod_reg);
            return 0;
        }
    }

    return 1;
}

errno_t
assemble_first(FILE *input_file) {
    if (input_file == NULL) {
        puts("Cannot read assembly file!");
        return 1;
    }

    int32_t binary_offset = 0;
    char raw_code[RAW_CODE_LENGTH];

    while (fgets(raw_code, RAW_CODE_LENGTH, input_file) != NULL) {
        instruction_t instruction;
        asm_code_t asm_code;

        int32_t parse_result, is_data_declare = 0;

        parse_result = parse_assembly(raw_code, &instruction, &asm_code);

        /* When parse error */
        if (parse_result == 2) {
            return 1;
        }

        if (parse_result == 0) {
            printf("%04X: %s", binary_offset, raw_code);
            binary_offset += strtol(instruction.instruction_code_length, NULL, 10);
            continue;
        }

        if (strncmp(asm_code.operator, "DW", 2) == 0) {
            strcpy(symbols[symbols_length].word_type, "w");
            is_data_declare = 1;
        }

        if (strncmp(asm_code.operator, "DB", 2) == 0) {
            strcpy(symbols[symbols_length].word_type, "b");
            is_data_declare = 1;
        }

        if (!is_data_declare) {
            printf("%04X: %s", binary_offset, raw_code);
            continue;
        }

        strcpy(symbols[symbols_length].name, asm_code.label);
        strcpy(symbols[symbols_length].data, asm_code.operand[0]);
        symbols[symbols_length].binary_offset = binary_offset;

        printf("%04X: %s", binary_offset, raw_code);

        switch (*symbols[symbols_length].word_type) {
            case 'w':
                binary_offset += 2;
                break;
            case 'b':
                binary_offset += 1;
                break;
            default:
                break;
        }

        ++symbols_length;
    }

    return 0;
}

errno_t
assemble_second(FILE *input_file, FILE *output_file) {
    return 0;
}

errno_t
main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    FILE *input_file;
    input_file = fopen(argv[1], "r");

    FILE *output_file;
    if (argc == 3) {
        output_file = fopen(argv[2], "w");
    } else {
        output_file = fopen("asm_out", "w");
    }

    init();

    puts("PASS 1 :");

    assemble_first(input_file);

    puts("PASS 2 :");

    return 0;
}