/*
 * Created by Chu Minseok on 23-11-25
 */

#ifndef SYSTEMPROGRAMMING_WEEK11_H
#define SYSTEMPROGRAMMING_WEEK11_H

#define ASSEMBLY_KEYWORD_DELIMETER " ,:\n\t"
#define RESERVED_WORDS_LENGTH      4
#define DATA_INIT_KEYWORDS_LENGTH  2

#include <stdint.h>

char* reserved_words[4] = {"ASSUME", "SEGMENT", "ENDS", "END"};
char* data_init_keywords[2] = {"DW", "DB"};

typedef struct {
    char name[6];
    uint16_t output_binary;
} instruction_info_t;

typedef struct {
    char name[3];
    uint8_t output_binary;
} register_info_t;

typedef struct {
    char name[15];
    int32_t binary_offset;
} symbol_info_t;

#endif /* SYSTEMPROGRAMMING_WEEK11_H */
