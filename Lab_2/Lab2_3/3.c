#include <stdio.h>
#include <stdlib.h>

typedef enum {
    SUCCESS = 0,
    ERROR_SUBSTRING_FOUND,
    ERROR_OPENING_FILE,
    ERROR_ALL_ZERO
} Status;

// Функция для поиска подстроки в строке (посимвольное сравнение)
int Find_substring(const char *line, const char *substring, int *position) {
    int line_len = 0, sub_len = 0;
    while (line[line_len] != '\0') line_len++;  // Вычисляем длину строки
    while (substring[sub_len] != '\0') sub_len++;  // Длина подстроки

    if (sub_len == 0) {
        return SUCCESS; // Пустая подстрока
    }

    for (int i = 0; i <= line_len - sub_len; i++) {
        int match = 1;
        for (int j = 0; j < sub_len; j++) {
            if (line[i + j] != substring[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            *position = i;
            return ERROR_SUBSTRING_FOUND; // Найдена подстрока
        }
    }
    return SUCCESS; // Подстрока не найдена
}

// Функция для обработки каждого файла
int Process_file(const char *filepath, const char *substring) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Не могу открыть файл\n");
        return ERROR_OPENING_FILE;
    }

    char line[1024];
    int line_number = 1;

    // Чтение файла построчно
    while (fgets(line, sizeof(line), file) != NULL) {
        int position = 0;
        int found = Find_substring(line, substring, &position);
        while (found) {
            printf("Файл: %s, Строка: %d, Позиция: %d\n", filepath, line_number, position + 1);
            found = Find_substring(line + position + 1, substring, &position); // Продолжаем поиск на оставшейся части строки
        }
        line_number++;
    }

    fclose(file);
    return SUCCESS;
}

// Основная функция для поиска подстроки в нескольких файлах
double Find_in_files(const char *substring, int file_count, const char *filepaths[]) {
    if (substring == NULL || substring[0] == '\0') {
        printf("Всё по нулям\n");
        return ERROR_ALL_ZERO;
    }

    for (int i = 0; i < file_count; i++) {
        double status = Process_file(filepaths[i], substring);
        if (status != 0) {
            printf("Ошибка при открытии файла: %s\n", filepaths[i]);
            return ERROR_OPENING_FILE;
        }
    }

    return SUCCESS;
}

// Пример использования функции
int main() {
    const char *substring = "example"; // Подстрока для поиска
    const char *files[] = {"file1.txt", "file2.txt"}; // Пути к файлам
    int file_count = sizeof(files) / sizeof(files[0]);

    double status = Find_in_files(substring, file_count, files);
    if (status != 0) {
        printf("Ошибка при поиске подстроки.\n");
        return ERROR_SUBSTRING_FOUND;
    }

    return 0;
}
