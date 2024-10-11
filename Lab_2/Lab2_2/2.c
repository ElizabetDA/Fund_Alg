#include <stdio.h>
#include <math.h>

double Geometric_mean(double *result, int count, const double numbers[]) {
    if (count <= 0) {
        return 1;
    }

    double product = 1.0;

    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0) {
            printf("Число должно быть положительным");
            return 1;
        }
        product *= numbers[i];
    }

    *result = pow(product, 1.0 / count); // корень степени count

    return 0;
}

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

    double status = Geometric_mean(&result, count, numbers);
    if (status == 0) {
        printf("Среднее геометрическое: %f\n", result);
    } else {
        printf("Ошибка при вычислении среднего геометрического.\n");
    }

    double base = 2.0;
    int exponent = 10;
    double power_result = Power(base, exponent);
    printf("%f в степени %d = %f\n", base, exponent, power_result);

    return 0;
}
