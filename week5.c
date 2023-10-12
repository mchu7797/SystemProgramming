//
// Created by minse on 2023-10-04.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_DATA_FILENAME "d.txt"
#define WORD_MAX_LENGTH 20

int wordCompare(const void *input1, const void *input2) {
    return strncmp(*((char**)input1), *((char**)input2), WORD_MAX_LENGTH);
}

int main(void) {
    FILE *input;

    if (fopen_s(&input, INPUT_DATA_FILENAME, "r")) {
        printf("Cannot open '%s'!\n", INPUT_DATA_FILENAME);
        return 1;
    }

    int inputSize;

    fseek(input, 0, SEEK_END);
    inputSize = ftell(input);
    fseek(input, 0, SEEK_SET);

    char *inputBinary = (char*)malloc(inputSize);
    char **wordList = (char**)malloc(sizeof(char*) * (inputSize - 1));

    fread(inputBinary, sizeof(char), inputSize, input);

    int wordListLength = 0;

    char *wordToken = strtok(inputBinary, " \n");
    while (wordToken) {
        *(wordList + wordListLength) = wordToken;
        wordListLength++;

        wordToken = strtok(NULL, " \n");
    }

    qsort(wordList, wordListLength, sizeof(char*), wordCompare);

    while (1) {
        char inputKeyword[20];

        puts("Keyword# ");
        scanf("%s", inputKeyword);

        if (strcmp(inputKeyword, "exit") == 0) {
            break;
        }

        for (int i = 0; i < wordListLength; i++) {
            if (strncmp(*(wordList + i), inputKeyword, WORD_MAX_LENGTH) == 0) {
                printf("'%s' -> %d/%d\n", inputKeyword, i + 1, wordListLength);
            }
        }
    }

    return 0;
}