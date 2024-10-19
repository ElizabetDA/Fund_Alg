#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    ERROR_ILLEGAL_CHARACTER,      // Недопустимый символ
    ERROR_MEMORY,                 //ошибка памяти
    ERROR_INVALID_SYMBOL,
    ERROR_INCORRECT_NUMBER_SYSTEM,
    ERROR_INVALID_INPUT,
    ERROR_IDDITION                //сложение
} Status;


int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_digit(char c, int base) {
    if (c >= '0' && c <= '9') {
        return (c - '0') < base; // Проверка для цифр
    } else if (c >= 'A' && c <= 'Z') {
        return (c - 'A' + 10) < base; // Проверка для букв
    }
    return ERROR_ILLEGAL_CHARACTER; 
}

char* add_in_column(const char* num1, const char* num2, int base) {            //сложение в столбик
    int len1 = str_len(num1);
    int len2 = str_len(num2);
    int max_len = (len1 > len2 ? len1 : len2) + 1; // +1 для возможного переноса
    char* result = (char*)malloc(max_len + 1); // +1 для символа конца строки
    if (result == NULL) {
        printf("Ошибка выделения памяти.\n");
    }
    result[max_len] = '\0'; // Завершаем строку

    int carry = 0; // Перенос

    // Сложение чисел с конца (начинаем с младших разрядов)
    for (int i = 0; i < max_len; i++) {
        int digit1 = 0;
        int digit2 = 0;

        // Получаем разряды из первого числа
        if (len1 - 1 - i >= 0) {
            if (num1[len1 - 1 - i] >= 'A') {
                digit1 = num1[len1 - 1 - i] - 'A' + 10;
            } else {
                digit1 = num1[len1 - 1 - i] - '0';
            }
        }

        // Получаем разряды из второго числа
        if (len2 - 1 - i >= 0) {
            if (num2[len2 - 1 - i] >= 'A') {
                digit2 = num2[len2 - 1 - i] - 'A' + 10;
            } else {
                digit2 = num2[len2 - 1 - i] - '0';
            }
        }

        // Суммируем разряды
        int sum = digit1 + digit2 + carry;
        carry = sum / base; // Определяем перенос
        sum = sum % base;   // Остаток

        // Преобразуем сумму обратно в символ
        if (sum < 10) {
            result[max_len - 1 - i] = sum + '0';
        } else {
            result[max_len - 1 - i] = sum + 'A' - 10;
        }
    }

    // Если есть остаточный перенос
    if (carry > 0) {
        if (carry < 10) {
            result[0] = carry + '0';
        } else {
            result[0] = carry + 'A' - 10;
        }
        return result; // Вернуть результат с переносом
    } else {
        // Удаляем ведущие нули, если они есть
        int leading_zeros = 0;
        while (result[leading_zeros] == '0' && leading_zeros < max_len) {
            leading_zeros++;
        }

        // Если все нули, возвращаем "0"
        if (leading_zeros == max_len) {
            result[0] = '0';
            result[1] = '\0';
            return result;
        }

        // Перемещаем результат влево, чтобы убрать ведущие нули
        for (int i = 0; i <= max_len - leading_zeros; i++) {
            result[i] = result[i + leading_zeros];
        }
        result[max_len - leading_zeros] = '\0'; // Завершаем строку
    }

    return result; // Возвращаем корректный результат
}

// Основная функция для суммирования чисел в произвольной системе счисления
char* sum_numbers(int base, int count_numbers, char numbers[][100]) {
    char* total_sum = (char*)malloc(2);
    if (total_sum == NULL) {
        printf("Ошибка выделения памяти.\n");
    }
    total_sum[0] = '0';
    total_sum[1] = '\0';

    for (int i = 0; i < count_numbers; i++) {
        // Проверка на корректность ввода
        for (int j = 0; j < str_len(numbers[i]); j++) {
            if (!is_valid_digit(numbers[i][j], base)) {
                printf("Ошибка: число %s содержит недопустимые символы для основания %d.\n", numbers[i], base);
                free(total_sum);
            }
        }

        char* new_sum = add_in_column(total_sum, numbers[i], base);
        free(total_sum); // Освобождаем старый указатель
        total_sum = new_sum; // Обновляем указатель на новый результат
    }

    return total_sum;
}

// Функция для удаления ведущих нулей из результата
char* remove_leading_zeros(char* result) {
    int i = 0;
    while (result[i] == '0' && result[i + 1] != '\0') {
        i++;
    }
    return result + i; // Возвращаем указатель на первый ненулевой символ
}

int main() {
    int base;
    printf("Введите основание системы счисления (от 2 до 36): ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("Основание системы должно быть в пределах от 2 до 36.\n");
        return ERROR_INCORRECT_NUMBER_SYSTEM;
    }

    int count_numbers;
    printf("Введите количество чисел для сложения: ");
    scanf("%d", &count_numbers);

    if (count_numbers <= 0) {
        printf("Чисел должно быть больше 0\n");
        return ERROR_INVALID_INPUT;
    }

    char numbers[count_numbers][100]; // Массив для хранения чисел

    for (int i = 0; i < count_numbers; i++) {
        printf("Введите число %d: ", i + 1);
        scanf("%s", numbers[i]);
    }

    char* result = sum_numbers(base, count_numbers, numbers);
    if (result) {
        char* trimmed_result = remove_leading_zeros(result);
        printf("Результат сложения: %s\n", trimmed_result);
        free(result - (result - trimmed_result)); // Освобождаем память, учтя сдвиг
    } else {
        printf("Ошибка в процессе сложения.\n");
        return ERROR_IDDITION;
    }

    return 0;
}