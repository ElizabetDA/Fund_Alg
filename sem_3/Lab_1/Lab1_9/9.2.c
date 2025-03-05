#include <stdio.h>
#include <stdlib.h>

// нахождение ближайшего элемента в массиве B к A[i]
int find_closest(int a_value, int *B, int size_B) {
    int closest = B[0];
    int min_diff = abs(a_value - B[0]);

    for (int i = 1; i < size_B; i++) {
        int diff = abs(a_value - B[i]);
        if (diff < min_diff) {
            min_diff = diff;
            closest = B[i];
        }
    }
    return closest;
}

int main() {
    unsigned int seed = 12345; 
    // случайный размер
    int size_A = 10 + rand() % 2001 - 1000;
    int size_B = 10 + rand() % 2001 - 1000;

    // память для массива
    int *A = (int *)malloc(size_A * sizeof(int));
    int *B = (int *)malloc(size_B * sizeof(int));

    // Проверка выделения памяти (надо ли??)
    if (A == NULL || B == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    // заполнение массивов
    for (int i = 0; i < size_A; i++) {
        A[i] = rand() % 2001 - 1000;
    }

    for (int i = 0; i < size_B; i++) {
        B[i] = rand() % 2001 - 1000;
    }

    // массив C
    int *C = (int *)malloc(size_A * sizeof(int));
    if (C == NULL) {
        printf("Ошибка выделения памяти для массива C!\n");
        free(A);
        free(B);
        return 1;
    }

    for (int i = 0; i < size_A; i++) {
        int closest_B = find_closest(A[i], B, size_B);
        C[i] = A[i] + closest_B;
    }

    printf("Массив C:\n");
    for (int i = 0; i < size_A; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    // освобождение памяти
    free(A);
    free(B);
    free(C);

    return 0;
}
