#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_PATH 4096
#define BUFFER_SIZE 4096

int is_number(const char *str) {
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

int xorN(const char **files, int file_count, int N) {
    int block_size = 1 << N;
    uint8_t *buffer = calloc(block_size, 1);
    if (!buffer) return 1;

    for (int i = 0; i < file_count; ++i) {
        FILE *f = fopen(files[i], "rb");
        if (!f) {
            perror("Ошибка открытия файла");
            free(buffer);
            return 1;
        }

        uint8_t *temp = calloc(block_size, 1);
        if (!temp) {
            fclose(f);
            free(buffer);
            return 1;
        }

        fread(temp, 1, block_size, f);
        for (int j = 0; j < block_size; ++j)
            buffer[j] ^= temp[j];

        free(temp);
        fclose(f);
    }

    printf("XOR result: ");
    for (int i = 0; i < block_size; ++i)
        printf("%02x", buffer[i]);
    printf("\n");

    free(buffer);
    return 0;
}

int mask_op(const char *filename, uint32_t hex_mask) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 1;

    uint32_t value;
    size_t count = 0;
    while (fread(&value, sizeof(uint32_t), 1, f) == 1)
        if ((value & hex_mask) == hex_mask) count++;

    fclose(f);
    printf("Файл: %s — совпадений: %zu\n", filename, count);
    return 0;
}

int copyN(const char *filename, int N) {
    for (int i = 0; i < N; ++i) {
        pid_t pid = fork();
        if (pid < 0) return 1;
        if (pid == 0) {
            char new_name[MAX_PATH];
            snprintf(new_name, MAX_PATH, "%s_copy%d", filename, i);
            FILE *src = fopen(filename, "rb");
            if (!src) exit(1);
            FILE *dst = fopen(new_name, "wb");
            if (!dst) {
                fclose(src);
                exit(1);
            }

            char buf[BUFFER_SIZE];
            size_t bytes;
            while ((bytes = fread(buf, 1, BUFFER_SIZE, src)) > 0)
                fwrite(buf, 1, bytes, dst);

            fclose(src);
            fclose(dst);
            exit(0);
        }
    }
    for (int i = 0; i < N; ++i) wait(NULL);
    return 0;
}

int find_string(const char *filename, const char *str) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char *line = NULL;
    size_t len = 0;
    int found = 0;

    while (getline(&line, &len, f) != -1)
        if (strstr(line, str)) { found = 1; break; }

    free(line);
    fclose(f);
    return found;
}

int find(const char **files, int count, const char *target) {
    int found_any = 0;
    for (int i = 0; i < count; ++i) {
        pid_t pid = fork();
        if (pid < 0) return 1;

        if (pid == 0) {
            if (find_string(files[i], target)) {
                printf("Найдено в: %s\n", files[i]);
                exit(0);
            } else exit(1);
        }
    }
    for (int i = 0; i < count; ++i) {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            found_any = 1;
    }

    if (!found_any)
        printf("Строка не найдена\n");

    return 0;
}

int main() {
    char input[4096];
    char *files[100];
    int file_count = 0;

    printf("Введите количество файлов: ");
    int n;
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; ++i) {
        printf("Файл %d: ", i + 1);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        files[i] = strdup(input);
    }

    printf("Выберите операцию [xorN / mask <hex> / copyN / find <строка>]:\n> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    if (strncmp(input, "xor", 3) == 0) {
        int N = atoi(input + 3);
        xorN((const char **)files, n, N);
    } else if (strncmp(input, "mask", 4) == 0) {
        char *hex = input + 5;
        uint32_t mask_val = strtoul(hex, NULL, 16);
        for (int i = 0; i < n; ++i)
            mask_op(files[i], mask_val);
    } else if (strncmp(input, "copy", 4) == 0) {
        int count = atoi(input + 4);
        for (int i = 0; i < n; ++i)
            copyN(files[i], count);
    } else if (strncmp(input, "find", 4) == 0) {
        char *str = input + 5;
        find((const char **)files, n, str);
    } else {
        printf("Неизвестная команда.\n");
    }

    for (int i = 0; i < n; ++i) free(files[i]);
    return 0;
}
