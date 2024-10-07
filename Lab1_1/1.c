#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Перечисление для флагов
typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

int GetOpts(int argc, char** argv, kOpts *option, int *number) {
    if (argc != 3) {
        return 1;
    }

    // первый аргумент - флаг
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        return 1;
    }

    switch (argv[1][1]) {
        case 'h':
            *option = OPT_H;
            break;
        case 'p':
            *option = OPT_P;
            break;
        case 's':
            *option = OPT_S;
            break;
        case 'e':
            *option = OPT_E;
            break;
        case 'a':
            *option = OPT_A;
            break;
        case 'f':
            *option = OPT_F;
            break;
        default:
            return 1;
    }

    // второй аргумент - число
    *number = atoi(argv[2]);
    if (*number == 0 && argv[2][0] != '0') {
        return 1;
    }

    return 0;
}

void HandlerOptH(int number) {
    int found = 0;
    for (int i = 1; i <= 100; ++i) {
        if (i % number == 0) {
            printf("%d\n", i);
            found = 1;
        }
    }
    if (!found) {
        printf("Нет чисел, кратных %d в пределах 100.\n", number);
    }
}

void HandlerOptP(int number) {
    if (number < 2) {
        printf("%d не является простым числом.\n", number);
        return;
    }

    int is_prime = 1;
    for (int i = 2; i <= sqrt(number); ++i) {
        if (number % i == 0) {
            is_prime = 0;
            break;
        }
    }

    if (is_prime) {
        printf("%d является простым числом.\n", number);
    } else {
        printf("%d является составным числом.\n", number);
    }
}

void From10to16(int number, char* result) {
    char tmp[100];
    int index = 0;

    while (number > 0) {
        if (number % 16 < 10) {
            tmp[index] = '0' + number % 16;
        } else {
            tmp[index] = 'A' + (number % 16 - 10);
        }
        index++;
        number /= 16;
    }
    for (int i = 0; i < index; i++) {
        result[i] = tmp[index - 1 - i];
    }
    result[index] = '\0';
}

void HandlerOptS(int number) {
    printf("Число в 16-ричной системе: ");
    char result[100];
    From10to16(number, result);

    for (int i = 0; result[i] != '\0'; ++i) {
        printf("%c ", result[i]);
    }
    putchar('\n');
}

void HandlerOptE(int number) {
    if (number > 10) {
        printf("Число x должно быть не больше 10 для флага -e.\n");
        return;
    }

    for (int base = 1; base <= 10; ++base) {
        printf("Основание %d:\n", base);
        for (int exp = 1; exp <= number; ++exp) {
            printf("%d^%d = %d\n", base, exp, (int)pow(base, exp));
        }
        printf("\n");
    }
}

void HandlerOptA(int number) {
    int sum = 0;
    for (int i = 1; i <= number; ++i) {
        sum += i;
    }
    printf("Сумма всех чисел от 1 до %d = %d\n", number, sum);
}

void HandlerOptF(int number) {
    unsigned long long factorial = 1;
    for (int i = 2; i <= number; ++i) {
        factorial *= i;
    }
    printf("Факториал %d = %llu\n", number, factorial);
}

int main(int argc, char** argv) {
    kOpts opt = OPT_H;
    int procceed_number = 0;

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("Некорректные аргументы. Используйте: ./program -<flag> <number>\n");
        return 1;
    }

    switch (opt) {
        case OPT_H:
            HandlerOptH(procceed_number);
            break;
        case OPT_P:
            HandlerOptP(procceed_number);
            break;
        case OPT_S:
            HandlerOptS(procceed_number);
            break;
        case OPT_E:
            HandlerOptE(procceed_number);
            break;
        case OPT_A:
            HandlerOptA(procceed_number);
            break;
        case OPT_F:
            HandlerOptF(procceed_number);
            break;
        default:
            printf("Некорректный флаг.\n");
            return 1;
    }

    return 0;
}
