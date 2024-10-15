#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    ERROR_NOT_CONVEX,
    ERROR_INVALID_VERTEX_COUNT,
    ERROR_INVALID_SYMBOL
} Status;

int orientation(double x1, double y1, double x2, double y2, double x3, double y3) {
    double val = (y2 - y1) * (x3 - x2) - (x2 - x1) * (y3 - y2);  // Определяем ориентацию трёх точек
    if (val > 0) return 1;    // Положительный поворот (по часовой стрелке)
    if (val < 0) return -1;   // Отрицательный поворот (против часовой стрелки)
    return 0;                 // Точки коллинеарны
}

Status is_convex_polygon(int n, ...) {    // Проверка на выпуклость
    va_list args;
    va_start(args, n);

    if (n < 6) {     //по двекоординаты на вершину (вершин должно быть хотя бы 3)
        va_end(args);
        return ERROR_INVALID_VERTEX_COUNT;
    }

    double x1 = va_arg(args, double);
    double y1 = va_arg(args, double);
    double x2 = va_arg(args, double);
    double y2 = va_arg(args, double);
    double x3, y3;
    int prev_orientation = 0;

    for (int i = 2; i < n / 2; i++) {
        x3 = va_arg(args, double);
        y3 = va_arg(args, double);

        int current_orientation = orientation(x1, y1, x2, y2, x3, y3);
        if (current_orientation != 0) {
            if (prev_orientation == 0) {
                prev_orientation = current_orientation;
            } else if (current_orientation != prev_orientation) {
                va_end(args);
                return ERROR_NOT_CONVEX; // Многоугольник не выпуклый
            }
        }

        x1 = x2;
        y1 = y2;
        x2 = x3;
        y2 = y3;
    }

    va_end(args);
    return SUCCESS; // Многоугольник выпуклый
}

double evaluate_polynomial(double x, int degree, ...) {   // x - точка, degree - степерь уравнения 
    va_list args;
    va_start(args, degree);

    double result = 0.0;
    for (int i = 0; i <= degree; i++) {         //i - текущий индекс коэффицента
        double coef = va_arg(args, double);     //вклад текущего коэффициента в значение многочлена
        result += coef * pow(x, degree - i);
    }

    va_end(args);
    return result;
}

// Вспомогательная функция: преобразование строки в число в системе счисления base
long long int str_to_int_base(const char *str, int base) {
    long long int result = 0;
    while (*str) {
        int digit = *str - '0';
        if (digit >= 0 && digit < base) {
            result = result * base + digit;
        } else {
            return ERROR_INVALID_SYMBOL;  // Некорректный символ для данного основания
        }
        str++;
    }
    return result;
}

// Проверка, является ли число Капрекара
int is_kaprekar(long long int n) {
    long long int square = n * n;

    // Определяем количество цифр в числе
    long long int divider = 1;
    while (divider <= n) {
        divider *= 10;
    }

    long long int left_part = square / divider;
    long long int right_part = square % divider;

    return (left_part + right_part == n);
}

// Основная функция: принимает основание и произвольное количество строковых аргументов
void check_kaprekar_numbers(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        const char *num_str = va_arg(args, const char *);
        long long int num = str_to_int_base(num_str, base);
        if (num == -1) {
            printf("Ошибка: неверный формат числа %s в системе с основанием %d\n", num_str, base);
            continue;
        }

        if (is_kaprekar(num)) {
            printf("Число %lld (в исходной системе счисления: %s) является числом Капрекара\n", num, num_str);
        } else {
            printf("Число %lld (в исходной системе счисления: %s) не является числом Капрекара\n", num, num_str);
        }
    }

    va_end(args);
}

int main() {
    printf("Часть 1: Проверка выпуклости многоугольника\n");
    Status polygon_status = is_convex_polygon(8, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0); //8 => 4-х угольник (8 кордитан)
    if (polygon_status == SUCCESS) {
        printf("Многоугольник выпуклый\n");
    } else if (polygon_status == ERROR_NOT_CONVEX) {
        printf("Многоугольник не выпуклый\n");
    } else if (polygon_status == ERROR_INVALID_VERTEX_COUNT) {
        printf("Ошибка: недостаточное количество вершин для многоугольника\n");
    }

    printf("\nЧасть 2: Вычисление значения многочлена\n");
    //double result = evaluate_polynomial(2.0, 2, 1.0, -3.0, 2.0); // Многочлен: 1x^2 - 3x + 2
    double result = evaluate_polynomial(78.0, 4, 1.0, -3.0, 2.0);
    printf("Значение многочлена в точке: %lf\n", result);

    printf("\nЧасть 3: Проверка чисел Капрекара\n");
    check_kaprekar_numbers(10, 3, "9", "45", "297");

    return 0;
}
