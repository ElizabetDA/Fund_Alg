#include "common.h"

/*
Клиент:
1. Открывает файл, указанный в аргументах командной строки.
2. Читает команды построчно \
3. Валидирует их синтаксис.
4. Отправляет команды серверу через очередь сообщений.
5. Получает ответ от сервера и выводит его в консоль.

Для демонстрации многопользовательской работы:
- Генерируем случайный clientId (или берём PID).
 */

#include "common.h"

static bool isCommandValid(const char *cmd) {
    size_t len = strlen(cmd);
    if (len == 0) return false;
    if (cmd[len - 1] != ';') return false;
    return true;
}

int main(int argc, char *argv[]) {
    // Проверяем, передан ли аргумент с файлом команд
    if (argc < 2) {
        fprintf(stderr, "Использование: ./client <файл_команд>\n");
        return 1;
    }

    key_t key = ftok(".", 'Q');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msgid = msgget(key, 0666);
    if (msgid < 0) {
        perror("msgget");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    // Генерируем ID клиента
    int clientId = getpid();

    char line[MAX_TEXT];
    while (fgets(line, sizeof(line), f)) {
        // Убираем символ перевода строки
        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        // зачистка концов строки
        for (int i = strlen(line) - 1; i >= 0; i--) {
            if (line[i] == ' ' || line[i] == '\t') {
                line[i] = '\0';
            } else {
                break;
            }
        }

        if (strlen(line) == 0) {
            continue;
        }

        // Проверяем синтаксис: должна оканчиваться на ';'
        if (!isCommandValid(line)) {
            fprintf(stderr, "Синтаксическая ошибка в команде: '%s'\n", line);
            continue;
        }

        // Убираем точку с запятой
        line[strlen(line) - 1] = '\0';

        Message req;
        req.mtype    = MSG_TYPE_REQUEST;
        req.clientId = clientId;
        strncpy(req.text, line, MAX_TEXT - 1);
        req.text[MAX_TEXT - 1] = '\0';

        if (msgsnd(msgid, &req, sizeof(Message) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            break;
        }

        Message resp;
        if (msgrcv(msgid, &resp, sizeof(Message) - sizeof(long), MSG_TYPE_RESPONSE, 0) < 0) {
            perror("msgrcv");
            break;
        }

         if (resp.clientId != clientId) {
            fprintf(stderr, "Предупреждение: получен ответ, не соответствующий нашему clientId\n");
        }

        printf("Сервер ответил: %s\n", resp.text);

        // Если пришла фатальная ошибка (кого-то «съели»), завершаем
        if (strstr(resp.text, "ФАТАЛЬНАЯ ОШИБКА")) {
            fprintf(stderr, "Завершение из-за фатальной ошибки.\n");
            break;
        }
    }

    fclose(f);
    return 0;
}
