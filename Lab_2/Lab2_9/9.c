#include <stdio.h>
#include <math.h>

// Функция для нахождения наибольшего общего делителя
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функция для проверки конечного представления десятичной дроби в системе счисления с основанием base
int has_finite_representation(long double number, int base) {
    // Шаг 1: Преобразуем дробь в несократимую форму
    long double fraction = number;
    
    // Пытаемся избавиться от дробной части
    while (fraction != floorl(fraction)) {
        fraction *= base;
    }

    // Шаг 2: Проверка остатков после деления
    while (fmodl(fraction, base) == 0) {
        fraction /= base;
    }

    // Если в итоге осталось 1, то представление конечно
    return (fraction == 1.0);
}


int main() {
    long double number = 0.5;
    int base;
    
    base = 2;
    int result = has_finite_representation(number, base);
    if (result) {
        printf("Число имеет конечное представление в системе счисления с основанием %d\n", base);
    } else {
        printf("Число не имеет конечного представления в системе счисления с основанием %d\n", base);
    }

    return 0;
}
