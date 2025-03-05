#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    INVALID_BASE = 1,
    INPUT_ERROR = 2
} Status;

int char_to_digit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;
}

// Перевод строки в число в заданной системе счисления
int convert_to_base10(const char *str, int base) {
    int value = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    }

    for (int i = 0; str[i] != '\0'; ++i) {
        int digit = char_to_digit(str[i]);
        if (digit >= base || digit < 0) {
            return 0;
        }
        value = value * base + digit;
    }

    return value * sign;
}

// Перевод числа в строку в новой системе счисления
void convert_from_base10(int number, int base, char *result) {
    int index = 0;
    int sign = (number < 0) ? -1 : 1;
    number = abs(number);

    do {
        int remainder = number % base;
        result[index++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
        number /= base;
    } while (number > 0);

    if (sign == -1) {
        result[index++] = '-';
    }

    result[index] = '\0';

    // Разворот строки
    for (int i = 0; i < index / 2; ++i) {
        char temp = result[i];
        result[i] = result[index - i - 1];
        result[index - i - 1] = temp;
    }
}

// Копирование строк
void copy_string(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// для ввода и проверки основания системы счисления
Status get_base(int *base) {
    char input[10];
    printf("Введите основание сс (2-36): ");

    // Чтение ввода как строки
    if (scanf("%s", input) != 1) {
        return INPUT_ERROR;
    }

    // Проверка, что строка состоит только из цифр
    int i = 0;
    while (input[i] != '\0') {
        if (input[i] < '0' || input[i] > '9') {
            return INPUT_ERROR;  // Строка содержит недопустимые символы
        }
        i++;
    }

    //преобразование строки в число вручную
    *base = 0;
    i = 0;
    while (input[i] != '\0') {
        *base = *base * 10 + (input[i] - '0');
        i++;
    }

    if (*base < 2 || *base > 36) {
        return INVALID_BASE;
    }

    return SUCCESS;
}

int main() {
    int base;
    if (get_base(&base) != SUCCESS) {
        printf("Ошибка: неверное основание сс\n");
        return INVALID_BASE;
    }

    char input[100];
    char max_input[100];  // Для хранения строки максимального числа
    int max_number = 0;
    int current_number;

    printf("Введите числа в сс %d. Для завершения введите 'Stop':\n", base);
    while (1) {
        scanf("%s", input);

        if (input[0] == 'S' && input[1] == 't' && input[2] == 'o' && input[3] == 'p') {
            break;
        }

        current_number = convert_to_base10(input, base);
        if (abs(current_number) > abs(max_number)) {
            max_number = current_number;
            copy_string(max_input, input);
        }
    }

    printf("Максимальное число в исходной сс: %s\n", max_input);
    printf("Максимальное число в десятичной сс: %d\n", max_number);

    // Дополнительно: вывод максимального числа в других системах счисления
    char result[100];

    convert_from_base10(max_number, 9, result);
    printf("В сс с основанием 9: %s\n", result);

    convert_from_base10(max_number, 18, result);
    printf("В сс с основанием 18: %s\n", result);

    convert_from_base10(max_number, 27, result);
    printf("В сс с основанием 27: %s\n", result);

    convert_from_base10(max_number, 36, result);
    printf("В сс с основанием 36: %s\n", result);

    return SUCCESS;
}
