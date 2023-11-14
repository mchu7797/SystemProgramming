//
// Created by minse on 2023-10-03.
//

#include <ctype.h>
#include <stdio.h>
#include <string.h>


#define SPEETCH_FILENAME "d.txt"
#define OUTPUT_FILENAME  "d.new.txt"

int
main(void) {
    FILE *speechFile, *outputFile;
    int character = 0;
    int characterHistory = 0;
    int wordCount = 0;

    int periodReached = 0;

    char keyword[20];
    char extractedWord[20];
    int extractedWordIndex = 0;

    int lineIndex = 0;
    int wordIndex = 0;

    speechFile = fopen(SPEETCH_FILENAME, "r");
    outputFile = fopen(OUTPUT_FILENAME, "w+");

    if (speechFile == NULL) {
        printf("Cannot open '%s'!\n", SPEETCH_FILENAME);
        return 1;
    }

    if (outputFile == NULL) {
        printf("Cannot write '%s'!\n", OUTPUT_FILENAME);
        return 1;
    }

    while ((character = fgetc(speechFile)) != EOF) {
        if (!isspace(character)) {
            if (character == '.') {
                periodReached = 1;
            }
            fputc(character, outputFile);
        } else if (characterHistory != ' ') {
            wordCount++;

            if (periodReached) {
                fputc('\n', outputFile);
                periodReached = 0;
                wordCount = 0;
            } else if (wordCount == 10) {
                fputc('\n', outputFile);
                wordCount = 0;
            } else {
                fputc(' ', outputFile);
            }
        }
        characterHistory = character;
    }

    fclose(speechFile);
    fclose(outputFile);

    speechFile = fopen(OUTPUT_FILENAME, "r");

    if (speechFile == NULL) {
        printf("Cannot open '%s'!\n", OUTPUT_FILENAME);
        return 1;
    }

    while (1) {
        printf("Keywords# ");
        scanf("%s", keyword);

        if (strncmp(keyword, "exit", 4) == 0) {
            break;
        }

        lineIndex = 0;
        wordIndex = 0;

        fseek(speechFile, 0, SEEK_SET);

        while ((character = fgetc(speechFile)) != EOF) {
            if (isspace(character) && !isspace(characterHistory)) {
                if (strncmp(extractedWord, keyword, 20) == 0) {
                    printf("Found!: (%d, %d)\n", lineIndex + 1, wordIndex + 1);
                }

                memset(extractedWord, 0, 20);
                extractedWordIndex = 0;

                if (character == '\n') {
                    lineIndex++;
                    wordIndex = 0;
                } else {
                    wordIndex++;
                }
            } else {
                extractedWord[extractedWordIndex++] = (char)character;
            }

            characterHistory = character;
        }
    }

    return 0;
}