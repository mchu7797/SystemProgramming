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
    int32_t is_dec, is_hex, i;
    char* string_ends;

    is_dec = 1;
    is_hex = 1;
    i = 0;

    if (string_text[0] == 0) {
        return -1;
    }

    while (string_text[i] != '\0') {
        if (!isdigit(string_text[i])) {
            is_dec = 0;
        }

        if (!isxdigit(string_text[i])) {
            if (string_text[i] == 'H' || string_text[i] == 'h') {
                string_ends = &string_text[i];
            } else {
                is_hex = 0;
            }
        }

        ++i;
    }

    if (is_dec) {
        return (uint32_t)strtol(string_text, NULL, 10);
    }

    if (is_hex) {
        return (uint32_t)strtol(string_text, &string_ends, 16);
    }

    return -1;
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
    uint32_t num;

    if (strcmp(operand, "CS") == 0) {
        return 8;
    }

    if (strcmp(operand, "DS") == 0) {
        return 9;
    }

    for (i = 0; i < registers_length; i++) {
        if (operand[0] == '[' && strncmp(operand + 1, registers[i].name, 3) == 0) {
            if (strncmp(registers[i].word_type, "w", 1) == 0) {
                /* Is word register pointer */
                return 6;
            }

            if (strncmp(registers[i].word_type, "b", 1) == 0) {
                /* Is byte register pointer */
                return 7;
            }
        } else if (strncmp(operand, registers[i].name, 3) == 0) {
            if (strncmp(registers[i].word_type, "w", 1) == 0) {
                /* Is word register */
                return 0;
            }

            if (strncmp(registers[i].word_type, "b", 1) == 0) {
                /* Is byte register */
                return 1;
            }
        }
    }

    num = string_to_int(operand);

    if (num != -1) {
        if (num > 255) {
            return 2;
        } else {
            return 3;
        }
    }

    if (operand[0] != '\0') {
        /* Is memory */
        return 4;
    } else {
        /* Is blank */
        return 5;
    }
}

int32_t
check_word_type_from_symbols(char* symbol_name) {
    int32_t i;

    for (i = 0; i < symbols_length; ++i) {
        if (strncmp(symbols[i].name, symbol_name, 10) != 0) {
            continue;
        }

        if (symbols[i].word_type[0] == 'w') {
            return 0;
        }

        if (symbols[i].word_type[0] == 'b') {
            return 1;
        }
    }

    return -1;
}

/* Cut raw string to keywords */
/* Assembly basic syntax: [Label] Mnemonic [Operands] [;Comments] */
errno_t
parse_assembly(char* raw_code, instruction_info_t* instruction_info, asm_sentence_t* parsed_code,
               int32_t check_symbols) {

    int32_t keywords_length, is_data_alloc, i, j, k;
    char raw_code_copied[RAW_CODE_LENGTH], *keywords[5], *token;

    keywords_length = 0;

    memset(raw_code_copied, 0, RAW_CODE_LENGTH);
    memset(keywords, 0, sizeof(char*) * 5);
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
        if (keywords[1] != NULL) {
            strcpy(parsed_code->label, keywords[0]);
            strcpy(parsed_code->operator, keywords[1]);
        } else {
            strcpy(parsed_code->operator, keywords[0]);
        }
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

    for (i = 0; i < RESERVED_WORDS_LENGTH; ++i) {
        if (strcmp(reserved_words[i], parsed_code->operator) == 0) {
            return 1;
        }
    }

    switch (analyze_operand(parsed_code->operand[0])) {
        case 0:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 1:
            strcpy(instruction_info->destination_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 2:
            strcpy(instruction_info->destination_memory, "i");
            strcpy(instruction_info->word_type, "w");
            break;
        case 3:
            strcpy(instruction_info->destination_memory, "i");
            strcpy(instruction_info->word_type, "b");
            break;
        case 4: strcpy(instruction_info->destination_memory, "m"); break;
        case 6:
            strcpy(instruction_info->destination_memory, "p");
            strcpy(instruction_info->word_type, "w");
            break;
        case 7:
            strcpy(instruction_info->destination_memory, "p");
            strcpy(instruction_info->word_type, "b");
            break;
        case 8:
            strcpy(instruction_info->destination_memory, "c");
            strcpy(instruction_info->word_type, "w");
            break;
        case 9:
            strcpy(instruction_info->destination_memory, "d");
            strcpy(instruction_info->word_type, "w");
            break;
        default: strcpy(instruction_info->destination_memory, "n"); break;
    }

    switch (analyze_operand(parsed_code->operand[1])) {
        case 0:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "w");
            break;
        case 1:
            strcpy(instruction_info->source_memory, "r");
            strcpy(instruction_info->word_type, "b");
            break;
        case 2:
            strcpy(instruction_info->source_memory, "i");
            strcpy(instruction_info->word_type, "w");
            break;
        case 3:
            strcpy(instruction_info->source_memory, "i");
            strcpy(instruction_info->word_type, "b");
            break;
        case 4: strcpy(instruction_info->source_memory, "m"); break;
        case 6:
            strcpy(instruction_info->source_memory, "p");
            strcpy(instruction_info->word_type, "w");
            break;
        case 7:
            strcpy(instruction_info->source_memory, "p");
            strcpy(instruction_info->word_type, "b");
            break;
        case 8:
            strcpy(instruction_info->source_memory, "c");
            strcpy(instruction_info->word_type, "w");
            break;
        case 9:
            strcpy(instruction_info->source_memory, "d");
            strcpy(instruction_info->word_type, "w");
            break;
        default: strcpy(instruction_info->source_memory, "n"); break;
    }

    if (check_symbols) {
        if (instruction_info->destination_memory[0] == 'm') {
            switch (check_word_type_from_symbols(parsed_code->operand[0])) {
                case 0: strcpy(instruction_info->word_type, "w"); break;
                case 1: strcpy(instruction_info->word_type, "b"); break;
                default:
                    if (instruction_info->source_memory[0] != 'n') {
                        puts("Can't recognize symbol type!");
                        return 1;
                    } else {
                        break;
                    }
            }
        } else if (instruction_info->source_memory[0] == 'm') {
            switch (check_word_type_from_symbols(parsed_code->operand[1])) {
                case 0: strcpy(instruction_info->word_type, "w"); break;
                case 1: strcpy(instruction_info->word_type, "b"); break;
                default: puts("Can't recognize symbol type!"); return 1;
            }
        }
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

    return 2;
}

errno_t
assemble_first(FILE* input_file) {
    asm_sentence_t asm_code;
    instruction_info_t instruction;

    int64_t location;
    int32_t is_data_declare, parse_response;
    char raw_code[RAW_CODE_LENGTH];

    if (input_file == NULL) {
        puts("Cannot read assembly file!");
        return 1;
    }

    location = 0;

    while (fgets(raw_code, RAW_CODE_LENGTH, input_file) != NULL) {
        is_data_declare = 0;

        /* When instruction found from instruction table */
        if (!parse_assembly(raw_code, &instruction, &asm_code, 0)) {
            printf("%04llX:%s", location, raw_code);
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
            printf("%04llX:%s", location, raw_code);
            continue;
        }

        strcpy(symbols[symbols_length].name, asm_code.label);
        strcpy(symbols[symbols_length].data, asm_code.operand[0]);
        symbols[symbols_length].binary_offset = location;

        printf("%04llX:%s", location, raw_code);

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

    int64_t location, i;
    int32_t parse_result;
    uint8_t address[3], number[3];
    char raw_code[RAW_CODE_LENGTH];

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
        if (!(parse_result = parse_assembly(raw_code, &instruction, &asm_code, 1))) {
            printf("%04llX:%s ", location, instruction.instruction_code);
            fputc((int32_t)strtol(instruction.instruction_code, NULL, 16), output_file);

            if (instruction.destination_memory[0] == 'r') {
                for (i = 0; i < registers_length; ++i) {
                    if (strncmp(registers[i].name, asm_code.operand[0], 3) != 0) {
                        continue;
                    }

                    strncpy(strchr(instruction.mod_reg, '?'), registers[i].number, 3);
                }
            }

            if (instruction.source_memory[0] == 'r') {
                for (i = 0; i < registers_length; ++i) {
                    if (strncmp(registers[i].name, asm_code.operand[1], 3) != 0) {
                        continue;
                    }

                    strncpy(strchr(instruction.mod_reg, '?'), registers[i].number, 3);
                }
            }

            if (instruction.destination_memory[0] == 'm' && instruction.source_memory[0] == 'n') {
                for (i = 0; i < symbols_length; ++i) {
                    if (strncmp(symbols[i].name, asm_code.operand[0], 10) != 0) {
                        continue;
                    }

                    strncpy(instruction.word_type, symbols[i].word_type, 1);

                    memset(address, 0, 3);
                    word_to_binary(symbols[i].binary_offset, address);

                    printf("%02X %02X %s", address[0], address[1], raw_code);
                    fwrite(address, 2, 1, output_file);
                }
            } else if (instruction.destination_memory[0] == 'n' && instruction.source_memory[0] == 'n') {
                printf("%02X %s", binary_to_int(instruction.mod_reg), raw_code);
                fputc((int32_t)binary_to_int(instruction.mod_reg), output_file);
            } else {
                if (instruction.destination_memory[0] == 'm') {
                    for (i = 0; i < symbols_length; ++i) {
                        if (strncmp(symbols[i].name, asm_code.operand[0], 10) != 0) {
                            continue;
                        }

                        if (instruction.word_type[0] != symbols[i].word_type[0]) {
                            puts("\nNot same word type between symbol and other operand!");
                            return 1;
                        } else {
                            strncpy(instruction.word_type, symbols[i].word_type, 2);
                        }

                        memset(address, 0, 3);
                        word_to_binary(symbols[i].binary_offset, address);

                        printf("%02X %02X %02X ", binary_to_int(instruction.mod_reg), address[0], address[1]);
                        fputc((int32_t)binary_to_int(instruction.mod_reg), output_file);
                        fwrite(address, 2, 1, output_file);
                    }
                } else if (instruction.source_memory[0] == 'm') {
                    for (i = 0; i < symbols_length; ++i) {
                        if (strncmp(symbols[i].name, asm_code.operand[1], 10) != 0) {
                            continue;
                        }

                        if (instruction.word_type[0] != symbols[i].word_type[0]) {
                            puts("Not same word type between symbol and other operand!");
                            return 1;
                        } else {
                            strncpy(instruction.word_type, symbols[i].word_type, 2);
                        }

                        memset(address, 0, 3);
                        word_to_binary(symbols[i].binary_offset, address);

                        printf("%02X %02X %02X ", binary_to_int(instruction.mod_reg), address[0], address[1]);
                        fputc((int32_t)binary_to_int(instruction.mod_reg), output_file);
                        fwrite(address, 2, 1, output_file);
                    }
                } else {
                    printf("%02X ", binary_to_int(instruction.mod_reg));
                    fputc((int32_t)binary_to_int(instruction.mod_reg), output_file);
                }

                memset(number, 0, 3);

                if (instruction.destination_memory[0] == 'i') {
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
                } else if (instruction.source_memory[0] == 'i') {
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
            }

            location += strtol(instruction.instruction_code_length, NULL, 10);
        } else {
            if (parse_result == 1) {
                continue;
            }

            for (i = 0; i < symbols_length; ++i) {
                if (strncmp(symbols[i].name, asm_code.label, 10) != 0) {
                    continue;
                }

                memset(number, 0, 3);

                if (strncmp(asm_code.operator, "DW", 2) == 0) {
                    word_to_binary(string_to_int(symbols[i].data), number);
                    printf("%04llX:%02X %02X %s", location, number[0], number[1], raw_code);
                    fwrite(number, 2, 1, output_file);
                    location += 2;
                }

                if (strncmp(asm_code.operator, "DB", 2) == 0) {
                    byte_to_binary(string_to_int(symbols[i].data), number);
                    printf("%04llX:%02X %s", location, number[0], raw_code);
                    fwrite(number, 1, 1, output_file);
                    ++location;
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