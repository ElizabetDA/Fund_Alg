#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdint.h>

// Макрос для вычисления размера блока 2^N
#define BLOCK_SIZE_2N(N) (1 << (N))

// Реализация операции xorN
void xorN_operation(int file_count, char *files[], int N) {
    size_t block_size = BLOCK_SIZE_2N(N); // Размер блока: 2^N
    uint8_t *block = malloc(block_size);
    if (!block) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < file_count; i++) {
        FILE *file = fopen(files[i], "rb");
        if (!file) {
            perror("Failed to open file");
            continue;
        }

        memset(block, 0, block_size);
        size_t bytes_read;
        while ((bytes_read = fread(block, 1, block_size, file)) > 0) {
            // Выполнение XOR для каждого байта
            for (size_t j = 0; j < bytes_read; j++) {
                block[j] ^= 0xFF; // Пример XOR с 0xFF
            }
            // Вывод результата
            fwrite(block, 1, bytes_read, stdout);
        }

        fclose(file);
    }

    free(block);
}

// Реализация операции mask
void mask_operation(int file_count, char *files[], uint32_t mask) {
    uint32_t value;
    int count = 0;

    for (int i = 0; i < file_count; i++) {
        FILE *file = fopen(files[i], "rb");
        if (!file) {
            perror("Failed to open file");
            continue;
        }

        while (fread(&value, sizeof(uint32_t), 1, file) == 1) {
            if ((value & mask) == mask) {
                count++;
            }
        }

        fclose(file);
    }

    printf("Count of matching values: %d\n", count);
}

// Реализация операции copyN
void copyN_operation(int file_count, char *files[], int N) {
    for (int i = 0; i < file_count; i++) {
        for (int j = 0; j < N; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                // Дочерний процесс
                char new_filename[256];
                snprintf(new_filename, sizeof(new_filename), "%s_%d", files[i], j + 1);
                FILE *src = fopen(files[i], "rb");
                FILE *dst = fopen(new_filename, "wb");
                if (!src || !dst) {
                    perror("Failed to open file");
                    exit(EXIT_FAILURE);
                }

                int ch;
                while ((ch = fgetc(src)) != EOF) {
                    fputc(ch, dst);
                }

                fclose(src);
                fclose(dst);
                exit(EXIT_SUCCESS);
            } else if (pid < 0) {
                perror("Failed to fork");
            }
        }

        // Ожидание завершения всех дочерних процессов
        for (int j = 0; j < N; j++) {
            wait(NULL);
        }
    }
}

// Реализация операции find
void find_operation(int file_count, char *files[], char *search_string) {
    int found = 0;

    for (int i = 0; i < file_count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Дочерний процесс
            FILE *file = fopen(files[i], "r");
            if (!file) {
                perror("Failed to open file");
                exit(EXIT_FAILURE);
            }

            char line[256];
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, search_string)) {
                    printf("Found in file: %s\n", files[i]);
                    found = 1;
                    break;
                }
            }

            fclose(file);
            exit(found ? EXIT_SUCCESS : EXIT_FAILURE);
        } else if (pid < 0) {
            perror("Failed to fork");
        }
    }

    // Ожидание завершения всех дочерних процессов
    for (int i = 0; i < file_count; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            found = 1;
        }
    }

    if (!found) {
        printf("String not found in any file.\n");
    }
}