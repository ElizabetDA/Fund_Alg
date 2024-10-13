#include <stdio.h>
#include <stdlib.h>

int convert_to_base(unsigned int number, unsigned int r, char *result, size_t size) {
    if (r < 1 || r > 5) {
        return -1; 
    }

    unsigned int base = 1 << r;  // сдвиг 1 на r битов (умножение 1 на 2^r)
    unsigned int mask = base - 1; // взятие остатка
    
    size_t pos = size - 1;
    result[pos] = '\0';

    while (number > 0 && pos > 0) {
        unsigned int remainder = number & mask; // остаток
        result[--pos] = (remainder < 10) ? ('0' + remainder) : ('A' + (remainder - 10)); // символ
        number >>= r; // деление на 2^r
    }

    if (number > 0) {
        return -2; // переполнение буфера
    }

    if (pos > 0) {
        for (size_t i = 0; i < size - pos; ++i) {
            result[i] = result[i + pos];
        }
    }

    return 0;
}

int main() {
    unsigned int number = 255;  
    unsigned int r = 3;        
    char result[33];           

    int status = convert_to_base(number, r, result, sizeof(result));
    if (status == 0) {
        printf("Результат: %s\n", result);
    } else if (status == -1) {
        printf("Ошибка: недопустимое основание.\n");
    } else if (status == -2) {
        printf("Ошибка: недостаточный размер буфера.\n");
    }

    return 0;
}
