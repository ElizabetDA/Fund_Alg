#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_LEN 1000

char to_lowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

void to_base4(int num, char *buffer) {
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 4) + '0';
        num /= 4;
    }
    buffer[i] = '\0';
}

void to_base8(int num, char *buffer) {
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 8) + '0';
        num /= 8;
    }
    buffer[i] = '\0';
}

// Чтение лексем из файла
int read_word(FILE *file, char *word) {
    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (i > 0) {
                word[i] = '\0';
                return 1;
            }
        } else {
            word[i++] = c;
        }
    }
    word[i] = '\0';
    return i > 0;
}

void process_t_flag(const char *file1_path, const char *file2_path, const char *output_path) {
    FILE *file1 = fopen(file1_path, "r");
    FILE *file2 = fopen(file2_path, "r");
    FILE *output = fopen(output_path, "w");

    if (file1 == NULL || file2 == NULL || output == NULL) {
        printf("Ошибка при открытии файлов.\n");
        return;
    }

    char word1[MAX_WORD_LEN], word2[MAX_WORD_LEN];
    int from_file1 = 1;

    while (1) {
        if (from_file1) {
            if (read_word(file1, word1)) {
                fprintf(output, "%s ", word1);
            } else if (!read_word(file2, word2)) {
                break;
            }
            from_file1 = 0;
        } else {
            if (read_word(file2, word2)) {
                fprintf(output, "%s ", word2);
            } else if (!read_word(file1, word1)) {
                break;
            }
            from_file1 = 1;
        }
    }

    fclose(file1);
    fclose(file2);
    fclose(output);
}


void process_a_flag(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    FILE *output = fopen(output_path, "w");

    if (input == NULL || output == NULL) {
        printf("Ошибка при открытии файлов.\n");
        return;
    }

    char word[MAX_WORD_LEN];
    int word_count = 0;

    while (read_word(input, word)) {
        word_count++;

        if (word_count % 10 == 0) {
            // Преобразуем в ASCII и выводим в cc 4
            for (int i = 0; word[i] != '\0'; i++) {
                char base4[10];
                to_base4((int) word[i], base4);
                fprintf(output, "%s", base4);
            }
        } else if (word_count % 2 == 0 && word_count % 10 != 0) {
            for (int i = 0; word[i] != '\0'; i++) {
                fprintf(output, "%c", to_lowercase(word[i]));
            }
        } else if (word_count % 5 == 0 && word_count % 10 != 0) {
            // Преобразуем в ASCII и выводим в cc 8
            for (int i = 0; word[i] != '\0'; i++) {
                char base8[10];
                to_base8((int) word[i], base8);
                fprintf(output, "%s", base8);
            }
        } else {
            fprintf(output, "%s", word);
        }

        fprintf(output, " ");
    }

    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Недостаточно аргументов.\n");
        return 1;
    }

    char *flag = argv[1];
    if (flag[0] != '-') {
        printf("Неверный формат флага.\n");
        return 1;
    }

    if (flag[1] == 't') {
        process_t_flag(argv[2], argv[3], argv[4]);
    } else if (flag[1] == 'a') {
        process_a_flag(argv[2], argv[3]);
    } else {
        printf("Неизвестный флаг: %s\n", flag);
        return 1;
    }

    return 0;
}
