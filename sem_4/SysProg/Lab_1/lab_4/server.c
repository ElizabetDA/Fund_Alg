#include "common.h"

/*
 * Сервер:
 1. Создаёт очередь сообщений.
 2. В бесконечном цикле ждёт запроса (команды) от клиента.
 3. Обрабатывает команду, меняет состояние (GameState).
 4. Проверяет, не произошло ли "съедение".
 5. Отправляет ответ клиенту.
 */

static void initState(GameState *st) {
    st->wolfSide     = SIDE_LEFT;
    st->goatSide     = SIDE_LEFT;
    st->cabbageSide  = SIDE_LEFT;
    st->farmerSide   = SIDE_LEFT;
}

static Side* getObjectSide(GameState *state, const char *objName) {
    if (strcmp(objName, "wolf") == 0)     return &state->wolfSide;
    if (strcmp(objName, "goat") == 0)     return &state->goatSide;
    if (strcmp(objName, "cabbage") == 0)  return &state->cabbageSide;
    return NULL;
}

static bool processCommand(GameState *state, const char *cmdText, char *outResponse) {
    // Разбираем команду по пробелам.
    // Возможные варианты:
    // 1) take wolf;
    // 2) put;
    // 3) move;
    // Упрощённо парсим:
    char command[16] = {0};
    char object[16]  = {0};

    int scanned = sscanf(cmdText, "%15s %15s", command, object);

    if (scanned <= 0) {
        snprintf(outResponse, MAX_TEXT, "Ошибка: пустая команда");
        return false;
    }

    // Смотрим, какая это команда
    if (strcmp(command, "take") == 0) {
        if (scanned < 2) {
            snprintf(outResponse, MAX_TEXT, "Ошибка: не указан объект в команде take");
            return false;
        }

        // Проверяем, есть ли место в лодке (лодке = farmerSide).
        // У нас в лодку можно взять только один объект, если он на том же берегу, что и фермер.
        Side *objSide = getObjectSide(state, object);
        if (!objSide) {
            snprintf(outResponse, MAX_TEXT, "Ошибка: неизвестный объект '%s'", object);
            return false;
        }

        // Объект и фермер должны быть на одном берегу, плюс объект не должен уже быть в лодке
        if (*objSide != state->farmerSide) {
            snprintf(outResponse, MAX_TEXT, "Ошибка: объект '%s' не на берегу фермера", object);
            return false;
        }
        // Проверяем, нет ли уже чего-то в лодке (кроме крестьянина)
        // Условно: если кто-то из wolf/goat/cabbage в SIDE_BOAT, значит лодка занята
        if (state->wolfSide   == SIDE_BOAT ||
            state->goatSide   == SIDE_BOAT ||
            state->cabbageSide== SIDE_BOAT) {
            snprintf(outResponse, MAX_TEXT, "Ошибка: лодка уже занята другим объектом");
            return false;
        }

        // Кладём объект в лодку
        *objSide = SIDE_BOAT;
        snprintf(outResponse, MAX_TEXT, "Успех: взяли '%s' в лодку", object);
        return true;

    } else if (strcmp(command, "put") == 0) {
        // Выкладываем из лодки всё, что там есть, на берег, где фермер
        // Ищем объект, у которого SIDE_BOAT
        bool found = false;
        if (state->wolfSide == SIDE_BOAT) {
            state->wolfSide = state->farmerSide;
            found = true;
        }
        if (state->goatSide == SIDE_BOAT) {
            state->goatSide = state->farmerSide;
            found = true;
        }
        if (state->cabbageSide == SIDE_BOAT) {
            state->cabbageSide = state->farmerSide;
            found = true;
        }

        if (!found) {
            snprintf(outResponse, MAX_TEXT, "Ошибка: в лодке ничего нет");
            return false;
        }
        snprintf(outResponse, MAX_TEXT, "Успех: выгрузили объект(ы) на берег");
        return true;

    } else if (strcmp(command, "move") == 0) {
        // Переплываем реку: меняем farmerSide
        if (state->farmerSide == SIDE_LEFT) {
            state->farmerSide = SIDE_RIGHT;
        } else {
            state->farmerSide = SIDE_LEFT;
        }
        snprintf(outResponse, MAX_TEXT, "Успех: переплыли на другой берег");
        return true;

    } else {
        snprintf(outResponse, MAX_TEXT, "Ошибка: неизвестная команда '%s'", command);
        return false;
    }
}

int main(void) {
    // Создаём ключ и очередь сообщений
    key_t key = ftok(".", 'Q'); // упрощённый вариант; в реальном проекте лучше уточнять путь
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid < 0) {
        perror("msgget");
        return 1;
    }

    printf("Сервер запущен. ID очереди: %d\n", msgid);

    GameState state;
    initState(&state);

    while (1) {
        // Ждём сообщения от клиентов
        Message msg;
        ssize_t rcvLen = msgrcv(msgid, &msg, sizeof(Message) - sizeof(long), MSG_TYPE_REQUEST, 0);
        if (rcvLen < 0) {
            perror("msgrcv");
            break;
        }

        // Обрабатываем команду
        char response[MAX_TEXT];
        bool ok = processCommand(&state, msg.text, response);

        // После изменения состояния проверяем, не нарушена ли безопасность
        if (ok && !isSafe(&state)) {
            ok = false;
            snprintf(response, MAX_TEXT, "ФАТАЛЬНАЯ ОШИБКА: кого-то съели! Игра проиграна.");
        }

        // Отправляем ответ
        Message resp;
        resp.mtype   = MSG_TYPE_RESPONSE;
        resp.clientId= msg.clientId;
        strncpy(resp.text, response, MAX_TEXT - 1);
        resp.text[MAX_TEXT - 1] = '\0'; // защита от переполнения

        if (msgsnd(msgid, &resp, sizeof(Message) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            break;
        }
    }

    // Удаляем очередь сообщений
    if (msgctl(msgid, IPC_RMID, NULL) < 0) {
        perror("msgctl");
    }
    printf("Сервер завершил работу.\n");
    return 0;
}
