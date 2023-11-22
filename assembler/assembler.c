/*
 * Created by minse on 2023-11-17.
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

#define RAW_CODE_LENGTH 50

/* Command information tables */
register_info_t registers[50];
instruction_info_t instructions[100];
symbol_info_t symbols[100];

/* Command information table's current length */
int32_t registers_length;
int32_t instructions_length;
int32_t symbols_length;

errno_t
init() {
    FILE *instruction_file, *register_file;
    int32_t i;

    instruction_file = fopen("instruction_list.txt", "r");
    register_file = fopen("register_list.txt", "r");

    if (instruction_file == NULL || register_file == NULL) {
        puts("Cannot open assembly keyword database!");
        return 1;
    }

    i = 0;

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

void
byte_to_binary(uint32_t input, uint8_t* output) {
    *output = (char)input;
}

void
word_to_binary(uint32_t input, uint8_t* output) {
    output[0] = input;
    output[1] = input >> 8;
}

uint32_t
string_to_int(char* string_text) {
    if (strchr(string_text, 'H') != NULL && !isalpha(string_text[0])) {
        return strtol(string_text, strchr(string_text, 'H'), 16);
    } else {
        return strtol(string_text, NULL, 10);
    }
}

uint32_t
binary_to_int(char* binary_text) {
    uint32_t result;
    int32_t i;

    i = 0;
    result = 0;

    while (*(binary_text + i) != '\0') {
        if (*(binary_text + i) == '1') {
            result += (int32_t)pow(2, (double)(strlen(binary_text + i) - 1));
        }

        ++i;
    }

    return result;
}

int32_t
analyze_operand(char* operand) {
    int32_t i;

    if (isdigit(operand[0])) {
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

    if (strcmp(operand, "") != 0) {
        /* Is memory */
        return 3;
    } else {
        /* Is blank */
        return 4;
    }
}

errno_t
parse_assembly(char* raw_code, instruction_info_t* instruction_info, asm_sentence_t* parsed_code) {
    /* Cut raw string to keywords */
    /* Assembly basic syntax: [Label] Mnemonic [Operands] [;Comments] */
    char raw_code_copied[RAW_CODE_LENGTH], *keywords[5], *token;
    int32_t keywords_length, i, j, k;

    keywords_length = 0;

    memset(raw_code_copied, 0, RAW_CODE_LENGTH);
    strcpy(raw_code_copied, raw_code);
    token = strtok(raw_code_copied, " ,\t\n");

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
        case 0: strcpy(instruction_info->destination_memory, "i"); break;
        case 1:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 2:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 3: strcpy(instruction_info->destination_memory, "m"); break;
        default: strcpy(instruction_info->destination_memory, "n"); break;
    }

    switch (analyze_operand(parsed_code->operand[1])) {
        case 0: strcpy(instruction_info->source_memory, "i"); break;
        case 1:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 2:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 3: strcpy(instruction_info->source_memory, "m"); break;
        default: strcpy(instruction_info->source_memory, "n"); break;
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
assemble_first(FILE* input_file) {
    instruction_info_t instruction;
    asm_sentence_t asm_code;

    char raw_code[RAW_CODE_LENGTH];
    int32_t is_data_declare, location;

    if (input_file == NULL) {
        puts("Cannot read assembly file!");
        return 1;
    }

    location = 0;

    while (fgets(raw_code, RAW_CODE_LENGTH, input_file) != NULL) {
        is_data_declare = 0;

        /* When instruction found from instruction table */
        if (!parse_assembly(raw_code, &instruction, &asm_code)) {
            printf("%04X:%s", location, raw_code);
            location += strtol(instruction.instruction_code_length, NULL, 10);
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
            printf("%04X:%s", location, raw_code);
            continue;
        }

        strcpy(symbols[symbols_length].name, asm_code.label);
        strcpy(symbols[symbols_length].data, asm_code.operand[0]);
        symbols[symbols_length].binary_offset = location;

        printf("%04X:%s", location, raw_code);

        switch (*symbols[symbols_length].word_type) {
            case 'w': location += 2; break;
            case 'b': location += 1; break;
            default: break;
        }

        ++symbols_length;
    }

    return 0;
}

errno_t
assemble_second(FILE* input_file, FILE* output_file) {
    instruction_info_t instruction;
    asm_sentence_t asm_code;

    char raw_code[RAW_CODE_LENGTH];
    uint8_t address[3], number[3];
    int32_t location, i;

    location = 0;

    if (input_file == NULL) {
        puts("Cannot read assembly file!");
        return 1;
    }

    if (output_file == NULL) {
        puts("Cannot write binary file!");
        return 1;
    }

    while (fgets(raw_code, RAW_CODE_LENGTH, input_file) != NULL) {
        if (!parse_assembly(raw_code, &instruction, &asm_code)) {
            printf("%04X:%02s ", location, instruction.instruction_code);
            fputc(strtol(instruction.instruction_code, NULL, 16), output_file);

            if (strncmp(instruction.destination_memory, "r", 1) == 0) {
                for (i = 0; i < registers_length; ++i) {
                    if (strncmp(registers[i].name, asm_code.operand[0], 3) != 0) {
                        continue;
                    }

                    strncpy(strchr(instruction.mod_reg, '?'), registers[i].number, 3);
                }
            }

            if (strncmp(instruction.source_memory, "r", 1) == 0) {
                for (i = 0; i < registers_length; ++i) {
                    if (strncmp(registers[i].name, asm_code.operand[1], 3) != 0) {
                        continue;
                    }

                    strncpy(strchr(instruction.mod_reg, '?'), registers[i].number, 3);
                }
            }

            if (strncmp(instruction.word_type, "n", 1) == 0) {
                printf("%02X %s", binary_to_int(instruction.mod_reg), raw_code);
                fputc(binary_to_int(instruction.mod_reg), output_file);
            } else {
                if (strncmp(instruction.destination_memory, "m", 1) == 0) {
                    for (i = 0; i < symbols_length; ++i) {
                        if (strncmp(symbols[i].name, asm_code.operand[0], 10) != 0) {
                            continue;
                        }

                        strncpy(instruction.word_type, symbols[i].word_type, 1);

                        memset(address, 0, 3);
                        word_to_binary(symbols[i].binary_offset, address);

                        printf("%02X %02X %02X ", binary_to_int(instruction.mod_reg), address[0], address[1], raw_code);
                        fputc(binary_to_int(instruction.mod_reg), output_file);
                        fwrite(address, 2, 1, output_file);
                    }
                } else if (strncmp(instruction.source_memory, "m", 1) == 0) {
                    for (i = 0; i < symbols_length; ++i) {
                        if (strncmp(symbols[i].name, asm_code.operand[1], 10) != 0) {
                            continue;
                        }

                        strncpy(instruction.word_type, symbols[i].word_type, 1);

                        memset(address, 0, 3);
                        word_to_binary(symbols[i].binary_offset, address);

                        printf("%02X %02X %02X ", binary_to_int(instruction.mod_reg), address[0], address[1], raw_code);
                        fputc(binary_to_int(instruction.mod_reg), output_file);
                        fwrite(address, 2, 1, output_file);
                    }
                } else {
                    printf("%02X ", binary_to_int(instruction.mod_reg), raw_code);
                    fputc(binary_to_int(instruction.mod_reg), output_file);
                }

                memset(number, 0, 3);

                if (strncmp(instruction.destination_memory, "i", 1) == 0) {
                    switch (instruction.word_type[0]) {
                        case 'w':
                            word_to_binary(string_to_int(asm_code.operand[0]), number);
                            printf("%02X %02X %s", number[0], number[1], raw_code);
                            fwrite(number, 2, 1, output_file);
                            break;
                        case 'b':
                            byte_to_binary(string_to_int(asm_code.operand[0]), number);
                            printf("%02X %s", number[0], raw_code);
                            fwrite(number, 1, 1, output_file);
                            break;
                        default: break;
                    }
                } else if (strncmp(instruction.source_memory, "i", 1) == 0) {
                    switch (instruction.word_type[0]) {
                        case 'w':
                            word_to_binary(string_to_int(asm_code.operand[1]), number);
                            printf("%02X %02X %s", number[0], number[1], raw_code);
                            fwrite(number, 2, 1, output_file);
                            break;
                        case 'b':
                            byte_to_binary(string_to_int(asm_code.operand[1]), number);
                            printf("%02X %s", number[0], raw_code);
                            fwrite(number, 1, 1, output_file);
                            break;
                        default: break;
                    }

                } else {
                    printf("%s", raw_code);
                }

                location += strtol(instruction.instruction_code_length, NULL, 10);
            }
        } else {
            for (i = 0; i < symbols_length; ++i) {
                if (strncmp(symbols[i].name, asm_code.label, 10) != 0) {
                    continue;
                }

                memset(number, 0, 3);

                if (strncmp(symbols[i].word_type, "w", 1) == 0) {
                    word_to_binary(string_to_int(symbols[i].data), number);
                    printf("%04X:%02X %02X %s", location, number[0], number[1], raw_code);
                    fputc(location, output_file);
                    fwrite(number, 2, 1, output_file);
                }

                if (strncmp(symbols[i].word_type, "b", 1) == 0) {
                    byte_to_binary(string_to_int(symbols[i].data), number);
                    printf("%04X:%02X %s", location, number[0], raw_code);
                    fputc(location, output_file);
                    fwrite(number, 1, 1, output_file);
                }

                if (*(symbols[i].word_type) == 'w') {
                    location += 2;
                } else if (*(symbols[i].word_type) == 'b') {
                    location += 1;
                }
            }
        }
    }

    return 0;
}

errno_t
main(int argc, char* argv[]) {
    FILE *input_file, *output_file;

    if (argc < 2) {
        return 1;
    }

    input_file = fopen(argv[1], "r");

    if (argc == 3) {
        output_file = fopen(argv[2], "w");
    } else {
        output_file = fopen("out.bin", "wb");
    }

    init();

    puts("PASS_1:");
    assemble_first(input_file);

    puts("PASS_2:");
    rewind(input_file);
    assemble_second(input_file, output_file);

    return 0;
}