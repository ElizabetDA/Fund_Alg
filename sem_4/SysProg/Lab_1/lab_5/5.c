#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

typedef enum { EMPTY, WOMEN, MEN } BathroomState;

typedef struct {
    BathroomState state;
    int count;
    int max_people;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Bathroom;

typedef struct {
    Bathroom *bathroom;
    int gender;
} ThreadArg;

void bathroom_init(Bathroom *bathroom, int max) {
    bathroom->state = EMPTY;
    bathroom->count = 0;
    bathroom->max_people = max;
    pthread_mutex_init(&bathroom->mutex, NULL);
    pthread_cond_init(&bathroom->cond, NULL);
}

void bathroom_destroy(Bathroom *bathroom) {
    pthread_mutex_destroy(&bathroom->mutex);
    pthread_cond_destroy(&bathroom->cond);
}

void woman_wants_to_enter(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);
    // Условие: ждем, если комната занята мужчинами или достигнут лимит
    while (bathroom->state == MEN || bathroom->count >= bathroom->max_people) {
        pthread_cond_wait(&bathroom->cond, &bathroom->mutex);
    }
    // Если комната пуста, устанавливаем состояние WOMEN
    if (bathroom->state == EMPTY) {
        bathroom->state = WOMEN;
    }
    bathroom->count++;
    printf("[OK] Woman entered. Count: %d (State: WOMEN)\n", bathroom->count);
    pthread_mutex_unlock(&bathroom->mutex);
}

void woman_leaves(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);
    bathroom->count--;
    printf("[OK] Woman left. Count: %d\n", bathroom->count);
    if (bathroom->count == 0) {
        bathroom->state = EMPTY;
        pthread_cond_broadcast(&bathroom->cond); // Уведомляем всех
    }
    pthread_mutex_unlock(&bathroom->mutex);
}

void man_wants_to_enter(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);
    // Условие: ждем, если комната занята женщинами или достигнут лимит
    while (bathroom->state == WOMEN || bathroom->count >= bathroom->max_people) {
        pthread_cond_wait(&bathroom->cond, &bathroom->mutex);
    }
    // Если комната пуста, устанавливаем состояние MEN
    if (bathroom->state == EMPTY) {
        bathroom->state = MEN;
    }
    bathroom->count++;
    printf("[OK] Man entered. Count: %d (State: MEN)\n", bathroom->count);
    pthread_mutex_unlock(&bathroom->mutex);
}

void man_leaves(Bathroom *bathroom) {
    pthread_mutex_lock(&bathroom->mutex);
    bathroom->count--;
    printf("[OK] Man left. Count: %d\n", bathroom->count);
    if (bathroom->count == 0) {
        bathroom->state = EMPTY;
        pthread_cond_broadcast(&bathroom->cond); // Уведомляем всех
    }
    pthread_mutex_unlock(&bathroom->mutex);
}

int get_max_people() {
    int max_people;
    printf("Enter maximum number of people in the bathroom: ");
    while (1) {
        if (scanf("%d", &max_people) != 1 || max_people <= 0) {
            fprintf(stderr, "Invalid input. N must be a positive integer.\nTry again: ");
            while (getchar() != '\n');
        } else {
            break;
        }
    }
    return max_people;
}

void* person_thread(void *arg) {
    ThreadArg *targ = (ThreadArg*)arg;
    Bathroom *bathroom = targ->bathroom;
    int gender = targ->gender;
    free(targ);

    if (gender == 0) { // Женщина
        woman_wants_to_enter(bathroom);
        printf("Woman entered. Count: %d\n", bathroom->count);
        sleep(rand() % 3 + 1);
        woman_leaves(bathroom);
    } else { // Мужчина
        man_wants_to_enter(bathroom);
        printf("Man entered. Count: %d\n", bathroom->count);
        sleep(rand() % 3 + 1);
        man_leaves(bathroom);
    }
    return NULL;
}

int main() {
    int max_people = get_max_people();
    Bathroom bathroom;
    bathroom_init(&bathroom, max_people);

    pthread_t threads[10];
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        ThreadArg *arg = malloc(sizeof(ThreadArg));
        if (!arg) {
            perror("malloc failed");
            bathroom_destroy(&bathroom);
            return EXIT_FAILURE;
        }
        arg->bathroom = &bathroom;
        arg->gender = rand() % 2; // Случайным образом выбираем пол
        if (pthread_create(&threads[i], NULL, person_thread, arg) != 0) {
            perror("pthread_create failed");
            free(arg);
            bathroom_destroy(&bathroom);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    bathroom_destroy(&bathroom);
    return EXIT_SUCCESS;
}
