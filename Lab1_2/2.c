#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

long double factorial(int n) {
	long double res = 1;
	for (int i = 2; i <= n; ++i) {
		res *= i;
	}
	return res;
}

long double to_double(char *string) {
	long double number = 0;
	int is_negative = 0;
	int decimal_places = 0;
	int is_decimal = 0;

	for (int j = 0; string[j] != '\0'; ++j) {
		if (string[j] == '-') {
			is_negative = 1;
		} else if (string[j] == '.') {
			is_decimal = 1;
		} else if (string[j] >= '0' && string[j] <= '9') {
			number = number * 10 + (string[j] - '0');
			if (is_decimal) {
				decimal_places++;
			}
		} else {
			return -1;
		}
	}

	while (decimal_places--) {
		number /= 10;
	}

	return is_negative ? -number : number;	
}

double_t Dychotomy(double_t x0, double_t x1, double_t (*func)(double_t), double EPSILON) {
    long double res = (x1 - x0) / 2;
	double last = -10e8;
    
	while(fabsl(func(res) -func(last)) >= EPSILON){
		if(func(res) > 0){
			x1 = res;
		}
		else{
			x0 = res;
		}
		last = res;
		res = (x0 + x1) / 2;
	}
	return res;
}


//E

long double limit_e(int n) {
	long double value = 1.0 + 1.0 / n;
	for (int j = 2; j <= n; ++j) {
		value *= (1.0 + 1.0 / n);
	}
	return value;
}

long double calculate_e_limit(long double eps) {
	long double value = limit_e(2);
	long double last_value;
	int n = 2;
	do {
		last_value = value;
		value = limit_e(n);
		n *= 2; //шаг 2, чтобы быстрее дойти
	} while (fabsl(value - last_value) >= eps); //fabsl - fabs для long double
	return value;
}

double exp_func(double_t x) {
	return log(x) - 1;
}

double exp_equation(double epsilon) {
    return Dychotomy(0, 3, exp_func, epsilon);
}

long double e_sequence(int n) {
	return 1.0 / factorial(n);
}

long double calculate_e_sequence(long double eps) {
	long double value = 0;
	long double last_value = 1;
	int n = 0;
	while (fabsl(last_value - value) >= eps) {
		last_value = value;
		value += e_sequence(n);
		n++;
	}
	return value;
}

//PI

long double lim_pi(int n) {
	long double value = 4.0;
	value *= 4.0 * n / ((2*n - 1) * (2*n - 1));
	value *= (long double)(n - 1);
	return value;
}

long double calculate_pi_limit(long double eps) {
	long double value = lim_pi(2);
	long double last_value;
	int n = 2;
	do {
		last_value = value;
		value = lim_pi(n);
		n += 1;
	} while (fabsl(value - last_value) >= eps);
	return value;
}

long double seq_pi(int n) {
	return 4.0 * (((n % 2 == 0) ? -1.0 : 1.0) / (2.0 * n - 1.0));
	//return 4.0 * ((n % 2) / (2.0 * n - 1.0));
}

long double calculate_pi_sequence(long double eps) {
	long double value = 0;
	long double last_value = 1;
	int n = 1;
	while (fabsl(last_value - value) >= eps) {
		last_value = value;
		value += seq_pi(n);
		n++;
	}
	return value;
}
double pi_func(double_t x) {
    return cos(x) + 1;
}

double pi_equation(double epsilon) {
    return Dychotomy(0, 2 * M_PI, pi_func, epsilon);
}

//LN

long double ln2_limit(int n) {
	return n * (pow(2, 1.0 / n) - 1);
}

long double calculate_ln2_limit(long double eps) {
	long double value = ln2_limit(1);
	long double last_value;
	int n = 1;
	do {
		last_value = value;
		value = ln2_limit(n);
		n *= 5;
	} while (fabsl(value - last_value) >= eps);
	return value;
}

long double ln2_seq(int n) {
	return (((n - 1) % 2 == 0) ? 1.0 : -1.0) / n;
}

long double calculate_ln2_sequence(long double eps) {
	long double value = 0;
	long double last_value = 1;
	int n = 1;
	while (fabsl(last_value - value) >= eps) {
		last_value = value;
		value += ln2_seq(n);
		n++;
	}
	return value;
}

double ln_func(double_t x) {
    return expl(x) - 2.0;
}

double ln_equation(double epsilon) {
    return Dychotomy(0, 3, ln_func, epsilon);
}

//SQPT

long double sqrt2_product(int k) {
    return pow(2, pow(2, -k));
}

long double calculate_sqrt2_product(long double eps) {
    long double product = 1.0;
    long double last_product = 0.0;
    int k = 2;

    while (fabsl(product - last_product) >= eps) {
        last_product = product;
        product *= sqrt2_product(k);
        k += 1;
    }
    return product;
}

double sqrt2_dech(double_t x) {
	return x * x - 2;
}

long double calculate_sqrt2_sequence(long double eps) {
    long double x = -0.5;
    long double x_next;
    while (1) {
        x_next = x - (x * x) / 2 + 1;
        if (fabsl(x_next - x) < eps) {
            break;
        }
        x = x_next;  // следующий
    }
    return x_next;
}


double sqrt_equation(double epsilon) {
    return Dychotomy(0, 5, sqrt2_dech, epsilon);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    // Преобразование строки в число с плавающей точкой
    long double epsilon = to_double(argv[1]);
    if (epsilon <= 0) {
        printf("Недопустимое значение эпсилона. Должно быть положительное число.\n");
        return 1;
    }

    printf("Вычисление с epsilon = %Lf\n", epsilon);

    long double e_val1 = calculate_e_sequence(epsilon);
    long double e_val2 = calculate_e_limit(epsilon);
    long double e_val3 = exp_equation(epsilon);

    long double pi_val1 = calculate_pi_sequence(epsilon);
    long double pi_val2 = calculate_pi_limit(epsilon);
    long double pi_val3 = pi_equation(epsilon);

    long double ln_val1 = calculate_ln2_limit(epsilon);
    long double ln_val2 = calculate_ln2_sequence(epsilon);
    long double ln_val3 = ln_equation(epsilon);

    long double sqrt_val1 = calculate_sqrt2_sequence(epsilon);
    long double sqrt_val2 = calculate_sqrt2_product(epsilon);
    long double sqrt_val3 = sqrt_equation(epsilon);

    // Вывод результатов
    printf("e (предел): %LF\n", e_val2);
    printf("e (ряд): %LF\n", e_val1);
    printf("e (уравнение): %LF\n", e_val3);

    printf("П (предел): %LF\n", pi_val2);
    printf("П (ряд): %LF\n", pi_val1);
    printf("П (уравнение): %LF\n", pi_val3);

    printf("ln(2) (предел): %LF\n", ln_val1);
    printf("ln(2) (ряд): %LF\n", ln_val2);
    printf("ln(2) (уравнение): %LF\n", ln_val3);

    printf("sqrt(2) (предел): %LF\n", sqrt_val3);
    printf("sqrt(2) (ряд): %LF\n", sqrt_val2);
    printf("sqrt(2) (уравнение): %LF\n", sqrt_val1);

    return 0;
}