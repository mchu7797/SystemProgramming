//
// Created by minse on 2023-10-11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SOURCE_FILENAME "input.asm"
#define MAX_KEYWORD_LENGTH 10
#define CMD_LIST_LENGTH 8
#define PCMD_LIST_LENGTH 6
#define BYTE_REGISTER_LIST_LENGTH 8
#define SHORT_REGISTER_LIST_LENGTH 13

char cmdList[CMD_LIST_LENGTH][MAX_KEYWORD_LENGTH] = {
        "MOV", "ADD", "JMP", "CMP",
        "INT", "JBE", "INC", "SUM"
};

char pcmdList[PCMD_LIST_LENGTH][MAX_KEYWORD_LENGTH] = {
        "ASSUME",
        "DEFINE",
        "WORD",
        "END",
        "ENDS",
        "SEGMENT"
};

char byteRegisterList[BYTE_REGISTER_LIST_LENGTH][MAX_KEYWORD_LENGTH] = {
        "AH", "AL",
        "BH", "BL",
        "CH", "CL",
        "DH", "DL"
};

char shortRegisterList[SHORT_REGISTER_LIST_LENGTH][MAX_KEYWORD_LENGTH] = {
        "AX", "BX", "CX",
        "DX", "CS", "IP",
        "SS", "SP", "BP",
        "SI", "DI", "DS",
        "ES"
};

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

    if (hexFlag > 0 && *(word + index - 1) != 'H') {
        return 0;
    }

    return 1;
}

int main(void) {
    FILE *asmFile = fopen(DEFAULT_SOURCE_FILENAME, "r");
    if (asmFile == NULL) {
        printf("Failed to open '%s'!\n", DEFAULT_SOURCE_FILENAME);
        return EXIT_FAILURE;
    }

    char asmLine[80], *token;
    while (fgets(asmLine, 80, asmFile) != NULL) {
        printf("%s", asmLine);
        printf("PARSED : ");

        token = strtok(asmLine, " ,:\n");

        while (token != NULL) {
            int tokenChecked = 0;

            for (int i = 0; i < CMD_LIST_LENGTH; i++) {
                if (strncmp(cmdList[i], token, MAX_KEYWORD_LENGTH) == 0) {
                    printf("CMD ");
                    tokenChecked = 1;
                }
            }

            for (int i = 0; i < PCMD_LIST_LENGTH; i++) {
                if (strncmp(pcmdList[i], token, MAX_KEYWORD_LENGTH) == 0) {
                    printf("PCMD ");
                    tokenChecked = 1;
                }
            }

            for (int i = 0; i < BYTE_REGISTER_LIST_LENGTH; i++) {
                if (strncmp(byteRegisterList[i], token, MAX_KEYWORD_LENGTH) == 0) {
                    printf("R8 ");
                    tokenChecked = 1;
                }
            }

            for (int i = 0; i < SHORT_REGISTER_LIST_LENGTH; i++) {
                if (strncmp(shortRegisterList[i], token, MAX_KEYWORD_LENGTH) == 0) {
                    printf("R16 ");
                    tokenChecked = 1;
                }
            }

            if (isDigit(token)) {
                printf("NUM ");
                tokenChecked = 1;
            }

            if (!tokenChecked) {
                printf("SYMB ");
            }

            token = strtok(NULL, " ,:\n");
        }

        printf("\n");
    }

    return EXIT_SUCCESS;
}