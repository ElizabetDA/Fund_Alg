#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int In(char letter, const char *alphabet) {
    for (size_t i = 0; i < str_len(alphabet); ++i) {
        if (letter == alphabet[i]) {
            return i;
        }
    }
    return -1;
}

int max(int value_1, int value_2) {
    return (value_1 > value_2) ? value_1 : value_2;
}

//из строки в десятичное представление
int to_decimal(const char *number, int base) {
    int result = 0;
    int len = str_len(number);

    for (int i = 0; i < len; i++) {
        int value = In(number[i], "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        if (value >= base) {
            return -1;
        }
        result = result * base + value;
    }

    return result;
}

void Solution(FILE *input, FILE *output) {
    char letter;
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char number[100]; // Буфер для хранения текущего числа
    int index = 0;

    while ((letter = fgetc(input)) != EOF) {
        if (letter == '\t' || letter == ' ' || letter == '\n') {
            // Если буфер не пуст, обрабатываем число
            if (index > 0) {
                number[index] = '\0'; // Завершаем строку
                int min_base = 2; // Минимальное основание
                int max_value = -1;

                // Определяем минимальное основание
                for (int i = 0; i < index; i++) {
                    int value = In(number[i], alphabet);
                    if (value == -1) {
                        fprintf(output, "Ошибка: неверный символ '%c'\n", number[i]);
                        return;
                    }
                    max_value = max(max_value, value);
                }

                min_base = max(max_value + 1, 2); // Минимальное основание сc
                if (min_base > 36) {
                    fprintf(output, "Число '%s' не может быть представлено в системе счисления от 2 до 36\n", number);
                } else {
                    int decimal_value = to_decimal(number, min_base);
                    fprintf(output, "%s %d %d\n", number, min_base, decimal_value);
                }

                index = 0; // Сбрасываем индекс для следующего числа
            }
        } else {
            // Добавляем символ к текущему числу
            number[index++] = letter;
        }
    }

    // Обрабатываем последнее число, если оно существует
    if (index > 0) {
        number[index] = '\0'; // Завершаем строку
        int min_base = 2; // Минимальное основание
        int max_value = -1;

        // Определяем минимальное основание
        for (int i = 0; i < index; i++) {
            int value = In(number[i], alphabet);
            if (value == -1) {
                fprintf(output, "Ошибка: неверный символ '%c'\n", number[i]);
                return;
            }
            max_value = max(max_value, value);
        }

        min_base = max(max_value + 1, 2); // Минимальное основание сc
        if (min_base > 36) {
            fprintf(output, "Число '%s' не может быть представлено в системе счисления от 2 до 36\n", number);
        } else {
            int decimal_value = to_decimal(number, min_base);
            fprintf(output, "%s %d %d\n", number, min_base, decimal_value);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *output_file = argv[2];

    FILE *input = fopen(input_file, "r");
    if (!input) {
        printf("Error: Cannot open input file %s\n", input_file);
        return 1;
    }

    FILE *output = fopen(output_file, "w");
    if (!output) {
        printf("Error: Cannot open or create output file %s\n", output_file);
        fclose(input);
        return 1;
    }

    Solution(input, output);

    fclose(input);
    fclose(output);
    return 0;
}