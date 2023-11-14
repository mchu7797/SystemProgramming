//
// Created by minse on 2023-09-20.
//

#include <stdio.h>

int
main(void) {
    char character = 'Q';
    int number = 72;
    char* string = "Phoenix";

    int string_length = 0;

    while (string[string_length] != 0) {
        string_length++;
    }

    printf("PRINTING LETTER '%c'\n", character);
    printf("%c\n%x\n%d\n", character, character, character);

    printf("PRINTING NUMBER '%d'\n", number);
    printf("%c\n%x\n%d\n", number, number, number);

    printf("PRINTING WORD '%s'\n", string);

    for (int i = 0; i < string_length; i++) {
        printf("%c", string[i]);
    }

    printf("\n");

    for (int i = 0; i < string_length; i++) {
        printf("%x ", string[i]);
    }

    printf("\n");

    for (int i = 0; i < string_length; i++) {
        printf("%d ", string[i]);
    }

    printf("\n");

    printf("PRINTING WORD '%s' AS BACKWARD\n", string);

    for (int i = string_length; i >= 0; i--) {
        printf("%c", string[i]);
    }

    printf("\n");

    return 0;
}