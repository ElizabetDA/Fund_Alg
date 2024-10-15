#include <stdio.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
} Status;

int Find_root_dichotomy(long double (*func)(long double), long double x0, long double x1, long double epsilon, long double *root) {
    long double result = (x1 - x0) / 2.0;
    long double last = -10e8;

    while (fabsl(func(result) - func(last)) >= epsilon) {
        if (func(result) > 0) {
            x1 = result;
        } else {
            x0 = result;
        }
        last = result;
        result = (x0 + x1) / 2.0;
    }

    *root = result;
    return SUCCESS;
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
    long double root;
    int status;

    //x^2 - 4 = 0 на интервале [0, 3]
    status = Find_root_dichotomy(equation1, 0, 3, 0.000001L, &root);
    if (status == 0) {
        printf("Корень уравнения x^2 - 4 = 0: %Lf\n", root);
    } else {
        printf("Ошибка в поиске корня для уравнения x^2 - 4 = 0\n");
    }

    // sin(x) - x/2 = 0 на интервале [1, 2]
    status = Find_root_dichotomy(equation2, 1, 2, 0.000001L, &root);
    if (status == 0) {
        printf("Корень уравнения sin(x) - x/2 = 0: %Lf\n", root);
    } else {
        printf("Ошибка в поиске корня для уравнения sin(x) - x/2 = 0\n");
    }

    return 0;
}
