#include <stdio.h>
#include <math.h>

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

    *result = pow(product, 1.0 / count); // Корень степени count
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
    double numbers[] = {2.0, 8.0, 10.0};
    int count = sizeof(numbers) / sizeof(numbers[0]);

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

    // Возведение в степень
    double base = 2.0;
    int exponent = 10;
    double power_result = Power(base, exponent);
    printf("%f в степени %d = %f\n", base, exponent, power_result);

    return 0;
}
