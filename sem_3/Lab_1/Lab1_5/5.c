#include <math.h>
#include <stdio.h>

// Функция для преобразования строки в число с плавающей запятой
double my_atof(const char* str) {
	double result = 0.0;
	double fraction = 0.0;
	int divisor = 1;
	int sign = 1;
	int i = 0;
	int fractional_part = 0;

	// Определение знака
	if (str[0] == '-') {
		sign = -1;
		i++;
	} else if (str[0] == '+') {
		i++;
	}

	// Обработка целой части
	for (; str[i] != '\0'; ++i) {
		if (str[i] == '.') {
			fractional_part = 1;
			continue;
		}
		if (str[i] >= '0' && str[i] <= '9') {
			if (!fractional_part) {
				result = result * 10 + (str[i] - '0');
			} else {
				fraction = fraction * 10 + (str[i] - '0');
				divisor *= 10;
			}
		} else {
			break;
		}
	}

	return sign * (result + fraction / divisor);
}

double factorial(int n) {
	double result = 1;
	for (int i = 2; i <= n; ++i) {
		result *= i;
	}
	return result;
}

double even_factorial(int n) {
	double result = 1;
	for (int i = 2; i <= n; i += 2) {
		result *= i;
	}
	return result;
}

double odd_factorial(int n) {
	double result = 1;
	for (int i = 1; i <= n; i += 2) {
		result *= i;
	}
	return result;
}

double sum_a(double x, double epsilon) {
	double sum = 0;
	double term = 1;  // Первый член ряда
	int n = 0;

	while (fabs(term) >= epsilon) {
		sum += term;
		++n;
		term = pow(x, n) / factorial(n);
	}

	return sum;
}

double sum_b(double x, double epsilon) {
	double sum = 0;
	double term = 1;  // Первый член ряда
	int n = 0;

	while (fabs(term) >= epsilon) {
		sum += term;
		++n;
		term = pow(-1, n) * pow(x, 2 * n) / factorial(2 * n);
	}

	return sum;
}

double sum_c(double x, double epsilon) {
	double sum = 0;
	double term = 1;  // Первый член ряда
	int n = 0;

	while (fabs(term) >= epsilon) {
		sum += term;
		++n;
		term = (pow(3, 3 * n) * factorial(n) * factorial(n) * factorial(n) * pow(x, 2 * n)) / factorial(3 * n);
	}

	return sum;
}

double sum_d(double x, double epsilon) {
	double sum = 0;
	double term = 1;  // Первый член ряда
	int n = 1;

	while (fabs(term) >= epsilon) {
		sum += term;
		term = pow(-1, n) * odd_factorial(2 * n - 1) * pow(x, 2 * n) / even_factorial(2 * n);
		++n;
	}

	return sum;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Эпсилон: %s <x> <epsilon>\n", argv[0]);
		return 1;
	}

	double x = my_atof(argv[1]);
	double epsilon = my_atof(argv[2]);

	printf("Сумма a: %lf\n", sum_a(x, epsilon));
	printf("Сумма b: %lf\n", sum_b(x, epsilon));
	printf("Сумма c: %lf\n", sum_c(x, epsilon));
	printf("Сумма d: %lf\n", sum_d(x, epsilon));

	return 0;
}
