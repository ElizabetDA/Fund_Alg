#include <stdio.h>
#include <stdlib.h>

size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

typedef enum {
    SUCCESS = 0,
    ERROR_UNKNOWN,
    ERROR_INVALID_SYMBOL
} Status;

Status len_string(const char* str, int* length) {
    if (str == NULL) {
        return ERROR_INVALID_SYMBOL;
    }

    *length = str_len(str);
    return SUCCESS;
}

Status reverse_string(const char* str, char** reversed) {
    if (str == NULL) {
        return ERROR_INVALID_SYMBOL;
    }

    size_t len = str_len(str);
    *reversed = (char*)malloc((len + 1) * sizeof(char));
    if (*reversed == NULL) {
        return ERROR_UNKNOWN;
    }

    for (size_t i = 0; i < len; i++) {
        (*reversed)[i] = str[len - 1 - i];
    }
    (*reversed)[len] = '\0';

    return SUCCESS;
}

// Функция для преобразования символов на нечётных позициях в верхний регистр
Status uppercase_odd_positions(const char* str, char** result) {
    if (str == NULL) {
        return ERROR_INVALID_SYMBOL;
    }

    size_t len = str_len(str);
    *result = (char*)malloc((len + 1) * sizeof(char));
    if (*result == NULL) {
        return ERROR_UNKNOWN;
    }

    for (size_t i = 0; i < len; i++) {
        if (i % 2 == 1 && str[i] >= 'a' && str[i] <= 'z') {
            (*result)[i] = str[i] - 32; // Преобразуем символ в верхний регистр
        } else {
            (*result)[i] = str[i];
        }
    }
    (*result)[len] = '\0';

    return SUCCESS;
}

// Основная функция
int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Ошибка: недостаточно аргументов\n");
        return ERROR_INVALID_SYMBOL;
    }

    char* flag = argv[1];
    char* line = argv[2];

    if (flag[0] == '-') {
        if (flag[1] == 'l') {
            int length = 0;
            Status status = len_string(line, &length);
            if (status == SUCCESS) {
                printf("Длина: %d\n", length);
            } else {
                printf("Ошибка: неверный символ\n");
            }
        } 
        else if (flag[1] == 'r') {
            char* reversed = NULL;
            Status status = reverse_string(line, &reversed);
            if (status == SUCCESS && reversed != NULL) {
                printf("Реверс: %s\n", reversed);
                free(reversed);
            } else {
                printf("Ошибка: не удалось реверсировать строку\n");
            }
        } 
        else {
            printf("Ошибка: неизвестный флаг %s\n", flag);
            return ERROR_UNKNOWN;
        }
    } else {
        printf("Ошибка: неверный формат флага\n");
        return ERROR_INVALID_SYMBOL;
    }

    return SUCCESS;
}
