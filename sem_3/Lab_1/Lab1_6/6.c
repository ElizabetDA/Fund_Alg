#include <stdio.h>
#include <math.h>

/*значение логарифма точные(вручную)
const float answer_1 = 0.8224670334241133;
const float answer_2 = 0.855624391892149;
const float answer_3 = 1.0;
const float answer_4 = 0.7834305107121345;
*/


double func_1(double x) {
    return log(x + 1) / x;
}

double func_2(double x) {
    return exp(-x * x / 2);
}

double func_3(double x) {
    return log(1 / (1 - x));
}

double func_4(double x) {
    return pow(x, x);
}

double trapezoidal(double a, double b, int number_op, double (*func)(double), double h) {
    double sum = 0.0;

    // Считаем начальные и конечные значения
    sum += func(a) / 2.0;
    sum += func(b) / 2.0;

    // Считаем сумму для всех остальных точек
    for (int i = 1; i < number_op; i++) {
        double x = a + i * h;
        sum += func(x);
    }

    return sum * h;
}


double my_atof(const char *str) {
    double result = 0.0;
    // Пропускаем начальные пробелы
    while (*str == ' ') str++;

    // Обработка знака
    if (*str == '+' || *str == '-') {
        if (*str == '-') return -1;
        str++;
    }

    // Чтение целой части
    while (*str >= '0' && *str <= '9') {
        result = result * 10.0 + (*str - '0');
        str++;
    }

    // Чтение дробной части
    if (*str == '.') {
        str++;
        double decimal_place = 0.1;
        while (*str >= '0' && *str <= '9') {
            result += (*str - '0') * decimal_place;
            decimal_place *= 0.1;
            str++;
        }
    }

    return result;
}


double runge(double a, double b, double epsilon, double (*func)(double)) {
    int number_op = 2;  // количество разбиений
    double h = (b - a) / number_op;
    double I_h = trapezoidal(a, b, number_op, func, h); // с шагом h

    number_op *= 2; // Уменьшаем
    h = (b - a) / number_op;
    double I_h2 = trapezoidal(a, b, number_op, func, h); // с шагом h/2

    //с точностью по Рунге
    while (fabs(I_h2 - I_h) / 3.0 > epsilon) {
        I_h = I_h2;
        number_op *= 2;
        h = (b - a) / number_op;
        I_h2 = trapezoidal(a, b, number_op, func, h);
    }

    return I_h2;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Ввод не верный.\n");
        return 1;
    }

    double epsilon = my_atof(argv[1]);
    if (epsilon < 0) {
        printf("Input correct provided\n");
    }

    double a = 0.0000000000000000001, b = 1.0 - 0.00000000000001;
    // берем значения 0.1 и 0.9 чтобы не происходило неправильных арифмитеческих операций

    double result_1 = runge(a, b, epsilon, func_1);  
    double result_2 = runge(a, b, epsilon, func_2); 
    double result_3 = runge(a, b, epsilon, func_3);  
    double result_4 = runge(a, b, epsilon, func_4); 

    printf("Result of integration A: %f\n", result_1);
    printf("Result of integration B: %f\n", result_2);
    printf("Result of integration C: %f\n", result_3);
    printf("Result of integration D: %f\n", result_4);
}