#include <stdio.h>
#include <stdlib.h>

// Подсчёт длины строки (реализация без использования string.h)
size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Подсчёт длины строки для флага -l
int len_string(const char* str) {
    return str_len(str);
}

// Реверс строки для флага -r
char* reverse_string(const char* str) {
    size_t len = str_len(str);
    char* reversed = (char*)malloc((len + 1) * sizeof(char));
    if (reversed == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
    return reversed;
}

// Преобразование нечётных символов в верхний регистр для флага -u
char* uppercase_odd_positions(const char* str) {
    size_t len = str_len(str);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    if (result == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        if (i % 2 == 1 && str[i] >= 'a' && str[i] <= 'z') {
            result[i] = str[i] - 32; // Преобразуем символ в верхний регистр
        } else {
            result[i] = str[i];
        }
    }
    result[len] = '\0';
    return result;
}

// Сортировка строки: цифры, буквы, остальные символы для флага -n
char* sort_string(const char* str) {
    size_t len = str_len(str);
    char* sorted = (char*)malloc((len + 1) * sizeof(char));
    if (sorted == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        return NULL;
    }

    size_t index = 0;

    // Добавляем цифры
    for (size_t i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            sorted[index++] = str[i];
        }
    }

    // Добавляем буквы
    for (size_t i = 0; i < len; i++) {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            sorted[index++] = str[i];
        }
    }

    // Добавляем остальные символы
    for (size_t i = 0; i < len; i++) {
        if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) {
            sorted[index++] = str[i];
        }
    }

    sorted[len] = '\0';
    return sorted;
}

// Конкатенация строк в псевдослучайном порядке для флага -c
char* concatenate_random(int seed, int argc, char** argv) {
    srand(seed);

    // Количество строк для конкатенации
    int count = argc - 3;  // Учитываем три первых аргумента
    if (count < 2) {
        printf("Недостаточно строк для конкатенации\n");
        return NULL;
    }

    // Выделяем память для результата
    size_t total_length = 0;
    for (int i = 3; i < argc; i++) {
        total_length += str_len(argv[i]);
    }

    char* result = (char*)malloc((total_length + 1) * sizeof(char));
    if (result == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        return NULL;
    }

    result[0] = '\0';  // Начинаем с пустой строки

    // Конкатенируем строки в случайном порядке
    int* indices = (int*)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        indices[i] = i + 3;  // Индексы строк начиная с 3-го аргумента
    }

    // Перемешиваем индексы
    for (int i = 0; i < count; i++) {
        int j = rand() % count;
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Конкатенируем строки
    for (int i = 0; i < count; i++) {
        for (size_t j = 0; j < str_len(argv[indices[i]]); j++) {
            result[str_len(result)] = argv[indices[i]][j];
        }
    }
    result[total_length] = '\0';

    free(indices);
    return result;
}

// Преобразование строки в unsigned int
unsigned int my_atof(const char *str) {
    unsigned int result = 0;
    while (*str == ' ') str++;
    if (*str == '+' || *str == '-') {
        if (*str == '-') return -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("ввод: <флаг> <строка>\n");
        return 1;
    }

    char* flag = argv[1];
    char* line = argv[2];

    if (flag[0] == '-') {
        if (flag[1] == 'l') {
            printf("Длина: %d\n", len_string(line));
        } 
        else if (flag[1] == 'r') {
            char* reversed = reverse_string(line);
            if (reversed != NULL) {
                printf("Реверс: %s\n", reversed);
                free(reversed);
            }
        } 
        else if (flag[1] == 'u') {
            char* result = uppercase_odd_positions(line);
            if (result != NULL) {
                printf("Верхний регистр на нечётных позициях: %s\n", result);
                free(result);
            }
        } 
        else if (flag[1] == 'n') {
            char* sorted = sort_string(line);
            if (sorted != NULL) {
                printf("Отсортировано: %s\n", sorted);
                free(sorted);
            }
        } 
        else if (flag[1] == 'c') {
            if (argc < 4) {
                printf("Ошибка: недостаточно аргументов для флага -c\n");
                return 1;
            }
            unsigned int seed = my_atof(argv[2]);
            char* concatenated = concatenate_random(seed, argc, argv);
            if (concatenated != NULL) {
                printf("Конкатенация: %s\n", concatenated);
                free(concatenated);
            }
        } 
        else {
            printf("Ошибка: неизвестный флаг %s\n", flag);
            return 1;
        }
    } else {
        printf("Ошибка: неверный формат флага\n");
        return 1;
    }

    return 0;
}

/*
./program -l "hello"
./program -r "world"
./program -u "example"
./program -n "a1b2c3"
./program -c 12 "first" "second" "third"
*/
