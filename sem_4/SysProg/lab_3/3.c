#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int N;  // Количество философов (будет задано пользователем)
sem_t *forks;  // Динамический массив семафоров для вилок
sem_t mutex;   // Семафор для ограничения доступа к вилкам
int meals_count = 3;  // Количество приемов пищи для каждого философа

void* philosopher(void* num) {
    int id = *((int*)num);
    int left_fork = id;
    int right_fork = (id + 1) % N;
    int meals_eaten = 0;

    while (meals_eaten < meals_count) {
        // Философ думает
        printf("Философ %d думает...\n", id + 1);
        sleep(1);

        // Философ пытается взять вилки
        sem_wait(&mutex);
        sem_wait(&forks[left_fork]);
        printf("Философ %d взял левую вилку %d\n", id + 1, left_fork + 1);
        sem_wait(&forks[right_fork]);
        printf("Философ %d взял правую вилку %d\n", id + 1, right_fork + 1);
        sem_post(&mutex);

        // Философ ест
        printf("Философ %d ест...\n", id + 1);
        sleep(1);
        meals_eaten++;

        // Философ кладет вилки обратно
        sem_post(&forks[left_fork]);
        printf("Философ %d положил левую вилку %d\n", id + 1, left_fork + 1);
        sem_post(&forks[right_fork]);
        printf("Философ %d положил правую вилку %d\n", id + 1, right_fork + 1);
    }

    printf("Философ %d завершил трапезу.\n", id + 1);
    return NULL;
}

int main() {
    char input[100];  // Буфер для ввода пользователя
    char *endptr;     // Указатель для проверки корректности ввода

    // Ввод количества философов
    printf("Введите количество философов: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Ошибка: не удалось прочитать ввод.\n");
        return 1;
    }

    // Преобразование строки в число
    errno = 0;  // Сброс errno перед вызовом strtol
    long N_long = strtol(input, &endptr, 10);  // Объявление и присваивание в одной строке

    // Проверка ошибок преобразования
    if (errno == ERANGE || N_long <= 1 || N_long > INT_MAX) {
        fprintf(stderr, "Ошибка: введите целое число больше 1 и меньше %d.\n", INT_MAX);
        return 1;
    }

    // Проверка, что вся строка была корректно преобразована
    if (endptr == input || *endptr != '\n') {
        fprintf(stderr, "Ошибка: ввод должен быть целым числом.\n");
        return 1;
    }

    N = (int)N_long;  // Присваиваем значение N

    // Выделение памяти для семафоров вилок
    forks = (sem_t*)malloc(N * sizeof(sem_t));
    if (forks == NULL) {
        fprintf(stderr, "Ошибка: не удалось выделить память для вилок.\n");
        return 1;
    }

    // Инициализация семафоров для вилок
    for (int i = 0; i < N; i++) {
        if (sem_init(&forks[i], 0, 1) != 0) {
            fprintf(stderr, "Ошибка: не удалось инициализировать семафоры.\n");
            free(forks);
            return 1;
        }
    }

    // Инициализация семафора для ограничения доступа к вилкам
    if (sem_init(&mutex, 0, N - 1) != 0) {
        fprintf(stderr, "Ошибка: не удалось инициализировать семафор mutex.\n");
        for (int i = 0; i < N; i++) {
            sem_destroy(&forks[i]);
        }
        free(forks);
        return 1;
    }

    // Создание потоков для философов
    pthread_t *philosophers = (pthread_t*)malloc(N * sizeof(pthread_t));
    int *ids = (int*)malloc(N * sizeof(int));
    if (philosophers == NULL || ids == NULL) {
        fprintf(stderr, "Ошибка: не удалось выделить память для потоков.\n");
        for (int i = 0; i < N; i++) {
            sem_destroy(&forks[i]);
        }
        free(forks);
        sem_destroy(&mutex);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        if (pthread_create(&philosophers[i], NULL, philosopher, &ids[i]) != 0) {
            fprintf(stderr, "Ошибка: не удалось создать поток для философа %d.\n", i + 1);
            for (int j = 0; j < i; j++) {
                pthread_cancel(philosophers[j]);
            }
            for (int j = 0; j < N; j++) {
                sem_destroy(&forks[j]);
            }
            free(forks);
            sem_destroy(&mutex);
            free(philosophers);
            free(ids);
            return 1;
        }
    }

    // Ожидание завершения потоков
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Освобождение ресурсов
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&mutex);
    free(forks);
    free(philosophers);
    free(ids);

    printf("Все философы завершили трапезу.\n");
    return 0;
}