//
// Created by Minseok Chu on 2023-09-20.
//

#include <ctype.h>
#include <stdio.h>


int
main(void) {
    FILE* speechFile = fopen("d.txt", "r");

    if (speechFile == NULL) {
        puts("Cannot open 'd.txt'!\n");
        return 1;
    }

    int wordCount = 0;
    int charCount = 0;
    int numCount = 0;

    int tempChar;
    int last = 0;

    while ((tempChar = fgetc(speechFile)) != EOF) {
        if (tempChar >= '0' && tempChar <= '9') {
            numCount++;
        }

        if (isspace(tempChar) && !isspace(last)) {
            wordCount++;
        }

        charCount++;
        last = tempChar;
    }

    if (!isspace(last) && charCount > 0) {
        wordCount++;
    }

    if (ferror(speechFile)) {
        puts("Error caused when reading file!");
        return 1;
    }

    fclose(speechFile);

    printf("Read Result\n");
    printf("Words		: %d\n", wordCount);
    printf("Characters	: %d\n", charCount);
    printf("Numbers		: %d\n", numCount);

    return 0;
}