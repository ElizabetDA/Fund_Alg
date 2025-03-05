#include <stdio.h>
#include <stdlib.h>


// преобразование строки в целое число
int string_to_int(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // знак
    if (str[0] == '-') {
        sign = -1;
        i++;
    }

    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            printf("Ошибка: неверный формат числа.\n");
            return 1;
        }
        i++;
    }

    return sign * result;
}

#define SIZE 10

// max min
void find_and_swap(int *arr, int size) {
    int min_index = 0, max_index = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }
    // обмен
    int temp = arr[min_index];
    arr[min_index] = arr[max_index];
    arr[max_index] = temp;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <a> <b>\n", argv[0]);
        return 1;
    }

    int a = string_to_int(argv[1]);
    int b = string_to_int(argv[2]);

    if (a >= b) {
        printf("Ошибка: a должно быть меньше b\n");
        return 1;
    }

    int arr[SIZE];

    // случайное число
    //unsigned int seed;
    for (int i = 0; i < SIZE; i++) {
        arr[i] = a + (rand() % (b - a + 1));
    }
    // до обмена
    printf("Массив до обмена:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Поиск и обмен
    find_and_swap(arr, SIZE);

    //после обмена
    printf("Массив после обмена:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
