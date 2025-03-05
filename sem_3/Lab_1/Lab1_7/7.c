#include <stdlib.h>
#include <stdio.h>

#define MAX_WORD_LEN 1000


int string_compare(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0; // строки не равны
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0'); // строки равны
}

char to_lowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

void to_base4(int num, char *buffer) {
    int i = 0;
    do {
        buffer[i++] = (num % 4) + '0';
        num /= 4;
    } while (num > 0);
    buffer[i] = '\0';
    // Реверсируем строку для правильного порядка
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

void to_base8(int num, char *buffer) {
    int i = 0;
    do {
        buffer[i++] = (num % 8) + '0';
        num /= 8;
    } while (num > 0);
    buffer[i] = '\0';
    // Реверсируем строку для правильного порядка
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

int read_word(FILE *file, char *word) {
    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF && i < MAX_WORD_LEN - 1) { // Ограничение на размер ??? maybe поможет
        if (c == ' ' || c == '\n' || c == '\t') {
            if (i > 0) {
                word[i] = '\0';
                return 1;
            }
        } else {
            word[i++] = c;
        }
    }
    if (i > 0) {
        word[i] = '\0';
        return 1;
    }
    return 0;
}

void process_r_flag(const char *file1_path, const char *file2_path, const char *output_path) {
    FILE *file1 = fopen(file1_path, "r");
    FILE *file2 = fopen(file2_path, "r");
    FILE *output = fopen(output_path, "w");

    if (file1 == NULL || file2 == NULL || output == NULL) {
        printf("Ошибка при открытии файлов\n");
        return;
    }

    char word1[MAX_WORD_LEN], word2[MAX_WORD_LEN];
    int from_file1 = 1;

    while (read_word(from_file1 ? file1 : file2, from_file1 ? word1 : word2)) {
        if (from_file1) {
            fprintf(output, "%s ", word1);
        } else {
            fprintf(output, "%s ", word2);
        }
        from_file1 = !from_file1; // Переключение источника
    }

    fclose(file1);
    fclose(file2);
    fclose(output);
}

void process_a_flag(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    FILE *output = fopen(output_path, "w");

    if (input == NULL || output == NULL) {
        printf("Ошибка при открытии файлов\n");
        return;
    }

    char word[MAX_WORD_LEN];
    int word_count = 0;

    while (read_word(input, word)) {
        word_count++;

        if (word_count % 10 == 0) {
            char base4[10];
            to_base4((int) word[0], base4); // Пример работы с первым символом
            fprintf(output, "%s ", base4);
        } else if (word_count % 2 == 0) {
            fprintf(output, "%c ", to_lowercase(word[0])); // Пример работы с первым символом
        } else if (word_count % 5 == 0) {
            char base8[10];
            to_base8((int) word[0], base8); // Пример работы с первым символом
            fprintf(output, "%s ", base8);
        } else {
            fprintf(output, "%s ", word);
        }
    }

    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Недостаточно аргументов\n");
        return 1;
    }

    char *flag = argv[1];
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Неверный формат флага\n");
        return 1;
    }

    if ((flag[1] == 'r' && argc < 5) || (flag[1] == 'a' && argc < 4)) {
        printf("Недостаточно аргументов\n");
        return 1;
    }

    if (string_compare(argv[2], argv[3]) || (flag[1] == 'r' && string_compare(argv[2], argv[4])) || (flag[1] == 'a' && string_compare(argv[2], argv[3]))) {
        printf("Ошибка: файлы ввода и вывода должны быть разными\n");
        return 1;
    }

    if (flag[1] == 'r') {
        process_r_flag(argv[2], argv[3], argv[4]);
    } else if (flag[1] == 'a') {
        process_a_flag(argv[2], argv[3]);
    } else {
        printf("Неизвестный флаг: %s\n", flag);
        return 1;
    }

    return 0;
}
