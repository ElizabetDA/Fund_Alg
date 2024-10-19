#include <stdio.h>
#include <stdarg.h>

typedef enum {
    SUCCESS = 0,
} Status;

void int_to_roman(int num, char *result);
void my_strcat(char *dest, const char *src);
void print_fibonacci_reverse(int n, FILE *stream);
void print_in_base(int num, int base, FILE *stream, int uppercase);
void print_memory_representation(const void *ptr, size_t size, FILE *stream);

// Пример обработки римских цифр
void int_to_roman(int num, char *result) {
    struct Roman {
        int value;
        const char *numeral;
    } roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
        {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"},
        {5, "V"}, {4, "IV"}, {1, "I"}, {0, NULL}
    };
    
    result[0] = '\0'; // Инициализируем пустую строку
    
    for (struct Roman *r = roman; r->value > 0; r++) {
        while (num >= r->value) {
            my_strcat(result, r->numeral);
            num -= r->value;
        }
    }
}

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++; // Найдем конец строки dest
    }
    while (*src) {
        *dest++ = *src++; // Копируем символы из src в dest
    }
    *dest = '\0'; // Завершаем строку
}

// Вывод чисел Фибоначчи в обратном порядке
void print_fibonacci_reverse(int n, FILE *stream) {
    if (n <= 0) 
        return;
    
    int a = 0, b = 1, temp;
    int fib[100]; // Максимальное количество чисел Фибоначчи
    int count = 0;

    for (int i = 0; i < n; i++) {
        fib[i] = a;
        temp = a + b;
        a = b;
        b = temp;
        count++;
    }

    // Выводим в обратном порядке
    for (int i = count - 1; i >= 0; i--) {
        fprintf(stream, "%d ", fib[i]);
    }
    fputc('\n', stream); // Завершение строки
}

// Вывод целого числа в заданной системе счисления
void print_in_base(int num, int base, FILE *stream, int uppercase) {
    if (num == 0) {
        fputc('0', stream);
        return;
    }
    char buffer[32];
    int i = 0;

    while (num > 0) {
        int digit = num % base;
        buffer[i++] = (digit < 10) ? '0' + digit : (uppercase ? 'A' + digit - 10 : 'a' + digit - 10);
        num /= base;
    }
    
    // Выводим в обратном порядке
    for (int j = i - 1; j >= 0; j--) {
        fputc(buffer[j], stream);
    }
}

// Вывод представления памяти
void print_memory_representation(const void *ptr, size_t size, FILE *stream) {
    const unsigned char *byte_ptr = (const unsigned char *)ptr;
    for (size_t i = 0; i < size; i++) {
        if (i > 0) fputc(' ', stream); // Разделяем пробелом
        fprintf(stream, "%02X", byte_ptr[i]); // Выводим в шестнадцатеричном формате с ведущим нулем (должно занимать не менее 2 символов)
    }
}

int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    const char *p = format;
    char temp[100];

    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 'r' && *(p + 1) == 'O') { // Пример римских цифр
                p += 2;
                int num = va_arg(args, int);
                int_to_roman(num, temp);
                fputs(temp, stream);
            } else if (*p == 'Z' && *(p + 1) == 'r') { // Число Фибоначчи
                p += 2;
                int n = va_arg(args, int);
                print_fibonacci_reverse(n, stream);
            } else if (*p == 'C') { // Система счисления
                p++;
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                print_in_base(num, base, stream, 0);
            } else if (*p == 'C' && *(p + 1) == 'V') { // Система счисления (uppercase)
                p += 2;
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                print_in_base(num, base, stream, 1);
            } else if (*p == 'o' && *(p + 1) == '%') { // Восьмеричная система
                p += 2;
                int num = va_arg(args, int);
                print_in_base(num, 8, stream, 0); // Восьмеричная система
            } else if (*p == 'T' && *(p + 1) == '0') { // Целое число
                p += 2;
                int num = va_arg(args, int);
                fprintf(stream, "%d", num);
            } else if (*p == 'm' && *(p + 1) == 'i') { // Представление памяти
                p += 2;
                int num = va_arg(args, int);
                print_memory_representation(&num, sizeof(num), stream);
            } else if (*p == 'm' && *(p + 1) == 'u') { // Представление памяти беззнакового
                p += 2;
                unsigned int num = va_arg(args, unsigned int);
                print_memory_representation(&num, sizeof(num), stream);
            } else if (*p == 'm' && *(p + 1) == 'd') { // Представление памяти double
                p += 2;
                double num = va_arg(args, double);
                print_memory_representation(&num, sizeof(num), stream);
            } else if (*p == 'm' && *(p + 1) == 'f') { // Представление памяти float
                p += 2;
                float num = va_arg(args, double); // float передается как double
                print_memory_representation(&num, sizeof(num), stream);
            }
        } else {
            fputc(*p, stream);
            p++;
        }
    }
    va_end(args);
    return SUCCESS;
}

int main() {
    overfprintf(stdout, "Число в римской записи: %rO\n", 42);
    overfprintf(stdout, "Число Фибоначчи (3 числа): %Zr\n", 3);
    overfprintf(stdout, "Число в шестнадцатеричной системе: %C, 255 16\n", 42, 16);
    overfprintf(stdout, "Число в восьмеричной системе: %o%\n", 42);
    overfprintf(stdout, "Целое число: %T0\n", 42);
    overfprintf(stdout, "Представление памяти (int): %mi\n", 42);
    overfprintf(stdout, "Представление памяти (unsigned int): %mu\n", 42u);
    overfprintf(stdout, "Представление памяти (double): %md\n", 42.0);
    overfprintf(stdout, "Представление памяти (float): %mf\n", 42.0f);
    
    return 0;
}
