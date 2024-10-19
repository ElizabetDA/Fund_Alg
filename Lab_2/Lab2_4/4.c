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
    double val = (y2 - y1) * (x3 - x2) - (x2 - x1) * (y3 - y2); 
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}

Status is_convex_polygon(int n, const double* coords) {
    if (n < 2) {
        return ERROR_INVALID_VERTEX_COUNT;
    }

    double x1 = coords[0];
    double y1 = coords[1];
    double x2 = coords[2];
    double y2 = coords[3];
    double x3, y3;
    int prev_orientation = 0;

    for (int i = 2; i < n; i++) {
        x3 = coords[2 * i];
        y3 = coords[2 * i + 1];

        int current_orientation = orientation(x1, y1, x2, y2, x3, y3);
        if (current_orientation != 0) {
            if (prev_orientation == 0) {
                prev_orientation = current_orientation;
            } else if (current_orientation != prev_orientation) {
                return ERROR_NOT_CONVEX;
            }
        }

        x1 = x2;
        y1 = y2;
        x2 = x3;
        y2 = y3;
    }

    return SUCCESS;
}

double evaluate_polynomial(double x, int degree, const double* coeffs) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, degree - i);
    }
    return result;
}

long long int str_to_int_base(const char *str, int base) {
    long long int result = 0;
    while (*str) {
        int digit = *str - '0';
        if (digit >= 0 && digit < base) {
            result = result * base + digit;
        } else {
            return ERROR_INVALID_SYMBOL;
        }
        str++;
    }
    return result;
}

int is_kaprekar(long long int n) {
    long long int square = n * n;
    long long int divider = 1;
    while (divider <= n) {
        divider *= 10;
    }
    long long int left_part = square / divider;
    long long int right_part = square % divider;
    return (left_part + right_part == n);
}

int main() {
    int vertex_count;
    printf("Часть 1: Проверка выпуклости многоугольника\n");
    if (scanf("%d", &vertex_count) != 1 && vertex_count < 2) {
        printf("Ошибка: недостаточное количество вершин для мн  огоугольника\n");
        return 1;
    }

    double* coords = (double*)malloc(vertex_count * 2 * sizeof(double));
    if (coords == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        return 1;
    }

    for (int i = 0; i < (vertex_count * 2); i++) {
        if (scanf("%lf", &coords[i]) != 1) {
            printf("Ошибка: неверный ввод координат\n");
            free(coords);
            return 1;
        }
    }

    Status polygon_status = is_convex_polygon(vertex_count, coords);
    if (polygon_status == SUCCESS) {
        printf("Многоугольник выпуклый\n");
    } else if (polygon_status == ERROR_NOT_CONVEX) {
        printf("Многоугольник не выпуклый\n");
    } else if (polygon_status == ERROR_INVALID_VERTEX_COUNT) {
        printf("Ошибка: недостаточное количество вершин для многоугольника\n");
    }

    printf("\nЧасть 2: Вычисление значения многочлена\n");
    int degree;
    if (scanf("%d", &degree) != 1 && degree < 0) {
        printf("Ошибка: некорректная степень многочлена\n");
        free(coords);
        return 1;
    }

    double* coeffs = (double*)malloc((degree + 1) * sizeof(double));
    if (coeffs == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        free(coords);
        return 1;
    }

    for (int i = 0; i <= degree; i++) {     
        if (scanf("%lf", &coeffs[i]) != 1) {
            printf("Ошибка: неверный ввод коэффициентов\n");
            free(coords);
            free(coeffs);
            return 1;
        }
    }

    double x;
    if (scanf("%lf", &x) != 1) {
        printf("Ошибка: неверный ввод значения x\n");
        free(coords);
        free(coeffs);
        return 1;
    }

    double poly_result = evaluate_polynomial(x, degree, coeffs);
    printf("Значение многочлена в точке: %lf\n", poly_result);

    printf("\nЧасть 3: Проверка чисел Капрекара\n");
    int base;
    int count;
    if (scanf("%d", &base) != 1 || scanf("%d", &count) != 1 || base < 2 || count < 1) {
        printf("Ошибка: неверное основание или количество чисел\n");
        free(coords);
        free(coeffs);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        char num_str[100];
        if (scanf("%s", num_str) != 1) {
            printf("Ошибка: неверный ввод числа\n");
            free(coords);
            free(coeffs);
            return 1;
        }

        long long int num = str_to_int_base(num_str, base);
        if (num == ERROR_INVALID_SYMBOL) {
            printf("Ошибка: неверный формат числа %s в системе с основанием %d\n", num_str, base);
            continue;
        }

        if (is_kaprekar(num)) {
            printf("Число %lld (в исходной системе счисления: %s) является числом Капрекара\n", num, num_str);
        } else {
            printf("Число %lld (в исходной системе счисления: %s) не является числом Капрекара\n", num, num_str);
        }
    }

    free(coords);
    free(coeffs);
    return 0;
}

/* 5
0.0 0.0 4.0 0.0 2.0 2.0 4.0 4.0 0.0 4.0
*/