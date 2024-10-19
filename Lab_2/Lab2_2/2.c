#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_COUNT,
    ERROR_NON_POSITIVE_NUMBER,
} Status;

// Функция для вычисления среднего геометрического
Status Geometric_mean(double* result, int count, const double numbers[]) {
    if (count <= 0) {
        return ERROR_INVALID_COUNT; // Неверное количество элементов
    }

    double product = 1.0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0) {
            return ERROR_NON_POSITIVE_NUMBER; // Число должно быть положительным
        }
        product *= numbers[i];
    }

    // Используем логарифмы для вычисления среднего геометрического
    *result = exp(log(product) / count);
    return SUCCESS;
}

// Функция для возведения числа в степень
double Power(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }
    if (exponent < 0) {
        return 1.0 / Power(base, -exponent);
    }
    double half = Power(base, exponent / 2);
    if (exponent % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}

int main() {
    double result;
    int count;

    // Ввод количества чисел
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Ошибка: Неверное количество чисел.\n");
        return 1; // Возврат кода ошибки 1 при некорректном вводе количества чисел
    }

    // Выделение памяти для массива
    double* numbers = (double*)malloc(count * sizeof(double));
    if (numbers == NULL) {
        printf("Ошибка: Не удалось выделить память.\n");
        return 1; // Возврат кода ошибки 1 при проблемах с выделением памяти
    }

    // Ввод чисел
    for (int i = 0; i < count; i++) {
        if (scanf("%lf", &numbers[i]) != 1) {
            printf("Ошибка: Неверное значение числа.\n");
            free(numbers);
            return 1; // Возврат кода ошибки 1 при неверном вводе числа
        }
        if (numbers[i] <= 0) {
            printf("Ошибка: Все числа должны быть положительными.\n");
            free(numbers);
            return 1; // Возврат кода ошибки 1 при отрицательном числе
        }
    }

    // Вычисляем среднее геометрическое
    Status status = Geometric_mean(&result, count, numbers);
    if (status == SUCCESS) {
        printf("Среднее геометрическое: %f\n", result);
    } else if (status == ERROR_INVALID_COUNT) {
        printf("Ошибка: Неверное количество чисел.\n");
    } else if (status == ERROR_NON_POSITIVE_NUMBER) {
        printf("Ошибка: Все числа должны быть положительными.\n");
    } else {
        printf("Неизвестная ошибка при вычислении среднего геометрического.\n");
    }

    // Ввод основания и показателя степени
    double base;
    int exponent;
    if (scanf("%lf", &base) != 1) {
        printf("Ошибка: Неверное значение основания.\n");
        free(numbers);
        return 1; // Возврат кода ошибки 1 при неверном вводе основания
    }

    if (scanf("%d", &exponent) != 1) {
        printf("Ошибка: Неверное значение показателя степени.\n");
        free(numbers);
        return 1; // Возврат кода ошибки 1 при неверном вводе показателя степени
    }

    // Возведение в степень
    double power_result = Power(base, exponent);
    printf("%f в степени %d = %f\n", base, exponent, power_result);

    // Освобождение выделенной памяти
    free(numbers);
    return 0;
}
