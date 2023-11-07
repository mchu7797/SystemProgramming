//
// Created by minse on 2023-09-20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_string(const char *first, const char *second, int size) {
    int compareFlag = 0;

    for (int i = 0; i < size; ++i) {
        if (*(first + i) == 0 || *(second + i) == 0)
            break;

        compareFlag += *(first + i) - *(second + i);
    }

    return compareFlag;
}

int main() {
    char *word_array = (char *) malloc(10 * 15);
    FILE *fruit_file_stream = fopen("fruits.txt", "r");

    if (fruit_file_stream == NULL) {
        printf("Cannot open 'fruits.txt'!");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        char temp[15];
        fscanf(fruit_file_stream, "%s", &temp);
        memcpy(word_array + 15 * i, temp, 15);
    }

    fclose(fruit_file_stream);

    while (1) {
        char input_word[15];

        printf("TYPE WORD FOR SEARCH INDEX : ");
        scanf("%s", input_word);

        if (compare_string("exit", input_word, 15) == 0) {
            break;
        }

        for (int i = 0; i < 10; i++) {
            if (compare_string(input_word, word_array + i * 15, 15) == 0) {
                printf("WORD FOUND! %s's INDEX IS %d\n", word_array + i * 15, i);

                printf("ANOTHER WORDS THAT STARTS TO '%c'\n", *(word_array + i * 15));

                for (int j = 0; j < 10; ++j) {
                    if (compare_string(word_array + i * 15, word_array + j * 15, 1) == 0) {
                        printf("%s\n", word_array + j * 15);
                    }
                }

                break;
            }
        }
    }

    free(word_array);

    return 0;
}