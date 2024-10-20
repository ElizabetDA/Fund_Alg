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

    for (int i = *position; i <= line_len - sub_len; i++) {
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
        printf("Не могу открыть файл: %s\n", filepath);
        return ERROR_OPENING_FILE;
    }

    char line[1024];
    int line_number = 1;

    // Чтение файла построчно
    while (fgets(line, sizeof(line), file) != NULL) {
        int position = 0;

        while (Find_substring(line, substring, &position) == ERROR_SUBSTRING_FOUND) {
            printf("Файл: %s, Строка: %d, Позиция: %d\n", filepath, line_number, position + 1);
            position++;  // Смещаем позицию для следующего поиска
        }

        line_number++;
    }

    fclose(file);
    return SUCCESS;
}

// Основная функция для поиска подстроки в нескольких файлах
int Find_in_files(const char *substring, int file_count, const char *filepaths[]) {
    if (substring == NULL || substring[0] == '\0') {
        printf("Всё по нулям\n");
        return ERROR_ALL_ZERO;
    }

    for (int i = 0; i < file_count; i++) {
        int status = Process_file(filepaths[i], substring);
        if (status != SUCCESS) {
            return status; // Возвращаем ошибку при открытии файла
        }
    }

    return SUCCESS;
}

int main() {
    char substring[256]; // Массив для хранения подстроки
    int file_count;

    // Ввод подстроки
    printf("Введите подстроку для поиска: ");
    scanf("%255s", substring); // Чтение строки, ограничивая длину
    printf("\n");

    // Ввод количества файлов
    printf("Введите количество файлов: ");
    scanf("%d", &file_count); // Чтение количества файлов
    printf("\n");

    // Проверка корректности введённого количества файлов
    if (file_count <= 0) {
        printf("Количество файлов должно быть больше нуля.\n");
        return ERROR_ALL_ZERO;
    }

    // Массив для хранения имен файлов
    char **files = malloc(file_count * sizeof(char *));
    if (files == NULL) {
        printf("Ошибка выделения памяти.\n");
        return ERROR_ALL_ZERO;
    }

    // Ввод имен файлов
    for (int i = 0; i < file_count; ++i) {
        files[i] = malloc(256 * sizeof(char)); // Выделение памяти для каждого имени файла
        if (files[i] == NULL) {
            printf("Ошибка выделения памяти для имени файла.\n");
            return ERROR_ALL_ZERO;
        }
        printf("Введите путь к файлу %d: ", i + 1);
        scanf("%255s", files[i]); // Чтение пути к файлу
        printf("\n");
    }

    // Поиск подстроки в файлах
    int status = Find_in_files(substring, file_count, (const char **)files);
    if (status != SUCCESS) {
        printf("Ошибка при поиске подстроки.\n");
        return status;
    }

    // Освобождение памяти
    for (int i = 0; i < file_count; ++i) {
        free(files[i]);
    }
    free(files);

    return SUCCESS;
}