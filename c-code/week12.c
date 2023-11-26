/*
 * Created by Chu Minseok on 23-11-25
 */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "week12.h"

instruction_info_t instruction_table[100];
register_info_t register_table[20];
symbol_info_t symbol_table[20];

int32_t instruction_table_length;
int32_t register_table_length;
int32_t symbol_table_length;

/*
 * Init : 프로그램을 실행하기 전 먼저 실행해야 하는 작업 실행
 * 리턴 값이 1일 경우 파일을 여는 데에 실패했다는 뜻. 
 */
errno_t
init() {
    FILE *instruction_file, *register_file;
    char binary_string[10];

    instruction_table_length = 0;
    register_table_length = 0;
    symbol_table_length = 0;

    instruction_file = fopen("instruction_12.txt", "r");
    register_file = fopen("register_12.txt", "r");

    if (instruction_file == NULL) {
        return 1;
    }

    if (register_file == NULL) {
        return 1;
    }

    while (!feof(instruction_file)) {
        fscanf(instruction_file, "%s%s\n", instruction_table[instruction_table_length].name, binary_string);
        instruction_table[instruction_table_length].output_binary = strtol(binary_string, NULL, 16);
        ++instruction_table_length;
    }

    while (!feof(register_file)) {
        fscanf(register_file, "%s%s\n", register_table[register_table_length].name, binary_string);
        register_table[register_table_length].output_binary = strtol(binary_string, NULL, 2);
        ++register_table_length;
    }

    return 0;
}

int32_t
is_numeric(char* string) {
    int32_t i = 0;

    /* 16진수는 무조건 0 또는 다른 숫자로 시작해야 함. */
    if (!isdigit(string[0])) {
        return 0;
    }

    while (string[i] != 0) {
        if (!isxdigit(string[i]) && string[i] != 'H' && string[i] != 'h') {
            break;
        } else {
            ++i;
        }
    }

    if (string[i] == 0) {
        return 1;
    }

    return 0;
}

errno_t
find_symbols(FILE* assembly_file) {
    int32_t i, is_symbol, binary_location, symbol_duplicated;
    char raw_code[80], *token;

    if (assembly_file == NULL) {
        return 1;
    }

    binary_location = 0;

    while (fgets(raw_code, 80, assembly_file) != NULL) {
        token = strtok(raw_code, ASSEMBLY_KEYWORD_DELIMETER);

        while (token != NULL) {
            if (strncmp(token, ";", 1) == 0) {
                token = strtok(NULL, ASSEMBLY_KEYWORD_DELIMETER);
                continue;
            }

            is_symbol = 1;

            for (i = 0; i < instruction_table_length; ++i) {
                if (strncmp(instruction_table[i].name, token, 6) != 0) {
                    continue;
                }
                is_symbol = 0;
            }

            for (i = 0; i < register_table_length; ++i) {
                if (strncmp(register_table[i].name, token, 3) != 0) {
                    continue;
                }
                is_symbol = 0;
            }

            for (i = 0; i < DATA_INIT_KEYWORDS_LENGTH; ++i) {
                if (strcmp(data_init_keywords[i], token) != 0) {
                    continue;
                }
                is_symbol = 0;
            }

            for (i = 0; i < RESERVED_WORDS_LENGTH; ++i) {
                if (strcmp(reserved_words[i], token) != 0) {
                    continue;
                }
                is_symbol = 0;
            }

            if (is_numeric(token)) {
                is_symbol = 0;
            }

            if (is_symbol) {
                symbol_duplicated = 0;

                for (i = 0; i < symbol_table_length; ++i) {
                    if (symbol_table[i].name[0] == 0) {
                        continue;
                    }

                    if (strncmp(symbol_table[i].name, token, 15) != 0) {
                        continue;
                    }

                    symbol_duplicated = 1;
                }

                if (!symbol_duplicated) {
                    strncpy(symbol_table[symbol_table_length].name, token, 15);
                    symbol_table[symbol_table_length].binary_offset = binary_location;
                    ++symbol_table_length;
                }
            }

            ++binary_location;
            token = strtok(NULL, ASSEMBLY_KEYWORD_DELIMETER);
        }
    }

    return 0;
}

errno_t
translate_to_binary(FILE* assembly_file) {
    int32_t i;
    char raw_code[80], *token;

    if (assembly_file == NULL) {
        return 1;
    }

    while (fgets(raw_code, 80, assembly_file) != NULL) {
        token = strtok(raw_code, ASSEMBLY_KEYWORD_DELIMETER);

        while (token != NULL) {
            for (i = 0; i < instruction_table_length; ++i) {
                if (strncmp(instruction_table[i].name, token, 6) != 0) {
                    continue;
                }

                printf("%02X ", instruction_table[i].output_binary);
            }

            for (i = 0; i < register_table_length; ++i) {
                if (strncmp(register_table[i].name, token, 3) != 0) {
                    continue;
                }

                printf("%02X ", register_table[i].output_binary);
            }

            for (i = 0; i < symbol_table_length; ++i) {
                if (strncmp(symbol_table[i].name, token, 15) != 0) {
                    continue;
                }

                printf("[%02X] ", symbol_table[i].binary_offset);
            }

            for (i = 0; i < DATA_INIT_KEYWORDS_LENGTH; ++i) {
                if (strcmp(data_init_keywords[i], token) != 0) {
                    continue;
                }

                printf("DIREC ");
            }

            for (i = 0; i < RESERVED_WORDS_LENGTH; ++i) {
                if (strcmp(reserved_words[i], token) != 0) {
                    continue;
                }

                printf("DIREC ");
            }

            if (is_numeric(token)) {
                printf("%s", token);
            }

            token = strtok(NULL, ASSEMBLY_KEYWORD_DELIMETER);
        }

        printf("\n");
    }

    return 0;
}

int32_t
main() {
    FILE* input_file;

    char* input_filename = "input.asm";

    switch (init()) {
        case 1: puts("명령어, 레지스터 테이블 파일을 불러올 수 없습니다!"); return EXIT_FAILURE;
        default: break;
    }

    input_file = fopen(input_filename, "r");

    if (input_file == NULL) {
        printf("'%s'을/를 열 수 없습니다!\n", input_filename);
        return EXIT_FAILURE;
    }

    if (find_symbols(input_file)) {
        printf("'%s'을/를 분석하는 도중 오류가 발생했습니다!\n", input_filename);
        return EXIT_FAILURE;
    }

    rewind(input_file);

    if (translate_to_binary(input_file)) {
        printf("'%s'을/를 변환하는 도중 오류가 발생했습니다!\n", input_filename);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}