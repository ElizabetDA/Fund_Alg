#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T
} kOpts;

// Сравнение вещественных чисел
int compare_float(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

// Решение квадратного уравнения
void solve_quadratic(double a, double b, double c, double epsilon) {
    double discriminant = b * b - 4 * a * c;

    if (compare_float(discriminant, 0.0, epsilon)) {
        printf("Одно решение: x = %.5lf\n", -b / (2 * a));
    } else if (discriminant > 0) {
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Два решения: x1 = %.5lf, x2 = %.5lf\n", root1, root2);
    } else {
        printf("Нет действительных решений\n");
    }
}

// Проверка кратности
void check_multiples(int a, int b) {
    if (b == 0) {
        printf("Ошибка: деление на ноль.\n");
        return;
    }
    if (a % b == 0) {
        printf("%d кратно %d\n", a, b);
    } else {
        printf("%d не кратно %d\n", a, b);
    }
}

// Проверка треугольного неравенства
void check_triangle(double a, double b, double c, double epsilon) {
    if (compare_float(a * a, b * b + c * c, epsilon) ||
        compare_float(b * b, a * a + c * c, epsilon) ||
        compare_float(c * c, a * a + b * b, epsilon)) {
        printf("Эти стороны могут образовать прямоугольный треугольник\n");
    } else {
        printf("Эти стороны не могут образовать прямоугольный треугольник\n");
    }
}

// целоое число для -m
int is_integer(const char* str) {
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i = 1;  // нужно пропускать знак
    }
    for (; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int GetOpts(int argc, char** argv, kOpts *option, double *params, int *param_count) {
    if (argc < 3) {
        return 1;
    }

    const char* procceding_option = argv[1];
    if (procceding_option[0] == '/' || procceding_option[0] == '-') {
        switch (procceding_option[1]) {
            case 'q':
                *option = OPT_Q;
                *param_count = 4;
                break;
            case 'm':
                *option = OPT_M;
                *param_count = 2;
                break;
            case 't':
                *option = OPT_T;
                *param_count = 4;
                break;
            default:
                return 1;
        }
    } else {
        return 1;
    }

    if (argc != (*param_count + 2)) {
        return 1;
    }

    for (int i = 0; i < *param_count; ++i) {
        const char* param_str = argv[i + 2];

        if (*option == OPT_M && !is_integer(param_str)) {  // Проверка на целое число для флага -m
            printf("Ошибка: параметр %s должен быть целым числом для флага -m.\n", param_str);
            return 1;
        }

        params[i] = atof(param_str);  // Преобразуем строку в число
    }

    return 0;
}

int main(int argc, char** argv) {
    kOpts opt;
    double params[4];
    int param_count = 0;

    if (GetOpts(argc, argv, &opt, params, &param_count)) {
        printf("Некорректные аргументы\n");
        return 1;
    }

    switch (opt) {
        case OPT_Q:
            solve_quadratic(params[1], params[2], params[3], params[0]);
            break;
        case OPT_M:
            check_multiples((int)params[0], (int)params[1]);
            break;
        case OPT_T:
            check_triangle(params[1], params[2], params[3], params[0]);
            break;
        default:
            printf("Некорректный флаг\n");
            return 1;
    }

    return 0;
}
