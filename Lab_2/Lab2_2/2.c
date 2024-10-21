#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include "geom_mean_power.h"

// Функция для вычисления среднего геометрического
int geometric_mean(double *result, int count, ...) {
    if (count <= 0 || result == NULL) {
        return -1; // Ошибка: неверные аргументы
    }

    va_list args;
    va_start(args, count);
    double product = 1.0;
    for (int i = 0; i < count; i++) {
        double value = va_arg(args, double);
        if (value <= 0) {
            va_end(args);
            return -2; // Ошибка: значения должны быть положительными
        }
        product *= value;
    }
    va_end(args);

    *result = pow(product, 1.0 / count);
    return 0; // Успех
}

// Функция быстрого возведения в степень
double power(double base, int exp) {
    if (exp == 0) {
        return 1.0;
    }
    if (exp < 0) {
        return 1.0 / power(base, -exp); // Обрабатываем отрицательную степень
    }
    double half = power(base, exp / 2);
    if (exp % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}
