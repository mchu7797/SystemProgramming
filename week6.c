//
// Created by minse on 2023-10-11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SOURCE_FILENAME "input.asm"

int isDigit(const char *word) {
    int hexFlag = 0;
    int index = 1;

    if (*word < '0' || *word > '9') {
        return 0;
    }

    while (*(word + index) != 0) {
        if (*(word + index) < '0' || *(word + index) > '9') {
            hexFlag++;
        }
        index++;
    }

    if (*(word + index - 1) != 'F') {
        return 0;
    }

    return 1;
}

int main(void) {
    FILE *input;

    // 어셈블리 명령어
    char *cmdTable[] = {
            "MOV",
            "ADD",
            "JMP",
            "CMP"
    };
    int cmdTableLength = 4;

    // 어셈블러 전처리 명령어
    char *pcmdTable[] = {
            "ASSUME",
            "DELETE",
            "WORD",
            "ENDS",
            "END",
            "SEGMENT"
    };
    int pcmdTableLength = 6;

    // 1 바이트 크기의 레지스터
    char *byteRegisterTable[] = {
            "AL", "AH",
            "BL", "BH",
            "CL", "CH",
            "DL", "DH"
    };
    int byteRegisterTableLength = 8;

    // 2 바이트 크기의 레지스터
    char *shortRegisterTable[] = {
            "AX", "BX", "CX", "DX"
    };
    int shortRegisterTableLength = 4;

    if (fopen_s(&input, DEFAULT_SOURCE_FILENAME, "r")) {
        printf("Failed to open '%s'!\n", DEFAULT_SOURCE_FILENAME);
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}