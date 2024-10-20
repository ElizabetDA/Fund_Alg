#include <stdio.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    ERROR_NULL_ROOT = 1,
    ERROR_INVALID_INTERVAL = 2,
} Status;

long double Dychotomy(long double x0, long double x1, long double (*func)(long double), long double EPSILON) {
    long double res = (x1 + x0) / 2.0;  // Изменено на (x1 + x0)
    long double last = -10e8;

    while (fabsl(func(res) - func(last)) >= EPSILON) {
        if (func(res) > 0) {
            x1 = res;
        } else {
            x0 = res;
        }
        last = res;
        res = (x0 + x1) / 2.0;  // Подсчет нового значения
    }
    
    return res;
}

// x^2 - 4 = 0
long double equation1(long double x) {
    return x * x - 4.0;
}

// sin(x) - x/2 = 0
long double equation2(long double x) {
    return sinl(x) - x / 2.0;
}

int main() {
    long double root1, root2;
    long double epsilon = 0.000001L;

    // x^2 - 4 = 0 на интервале [0, 3]
    root1 = Dychotomy(0, 3, equation1, epsilon);
    printf("Корень уравнения x^2 - 4 = 0: %Lf\n", root1);

    // sin(x) - x/2 = 0 на интервале [1, 2]
    root2 = Dychotomy(1, 2, equation2, epsilon);
    printf("Корень уравнения sin(x) - x/2 = 0: %Lf\n", root2);

    return 0;
}
