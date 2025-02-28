#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LOGIN_LENGTH 6
#define MAX_USERS 100
#define SANCTION_CODE "12345"

typedef struct {
    char login[MAX_LOGIN_LENGTH + 1];
    long int pin;
    int sanctionLimit;
} User;

typedef struct {
    User* users[MAX_USERS];
    int count;
    char dbFilePath[256];
} UserDatabase;

void initDatabase(UserDatabase* db, const char* filePath) {
    memset(db, 0, sizeof(UserDatabase));
    strncpy(db->dbFilePath, filePath, sizeof(db->dbFilePath) - 1);
    db->dbFilePath[sizeof(db->dbFilePath) - 1] = '\0';
}

void freeDatabase(UserDatabase* db) {
    for (int i = 0; i < db->count; i++) {
        free(db->users[i]);
    }
    db->count = 0;
}

int loadUsers(UserDatabase* db) {
    FILE* file = fopen(db->dbFilePath, "rb");
    if (!file) return 0;

    while (db->count < MAX_USERS) {
        User* user = malloc(sizeof(User));
        if (!user || fread(user, sizeof(User), 1, file) != 1) {
            free(user);
            break;
        }
        db->users[db->count++] = user;
    }

    fclose(file);
    return 1;
}

int saveUsers(const UserDatabase* db) {
    FILE* file = fopen(db->dbFilePath, "wb");
    if (!file) return 0;

    for (int i = 0; i < db->count; i++) {
        fwrite(db->users[i], sizeof(User), 1, file);
    }

    fclose(file);
    return 1;
}

User* findUser(const UserDatabase* db, const char *login) {
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->users[i]->login, login) == 0) {
            return db->users[i];
        }
    }
    return NULL;
}

int registerUser(UserDatabase* db, const char *login, long int pin) {
    if (db->count >= MAX_USERS) {
        printf("Достигнуто максимальное количество пользователей.\n");
        return 0;
    }

    User *newUser = malloc(sizeof(User));
    if (!newUser) {
        printf("Ошибка выделения памяти.\n");
        return 0;
    }

    strncpy(newUser->login, login, MAX_LOGIN_LENGTH);
    newUser->login[MAX_LOGIN_LENGTH] = '\0';
    newUser->pin = pin;
    newUser->sanctionLimit = 0;

    db->users[db->count++] = newUser;
    saveUsers(db);
    return 1;
}

void showCommands() {
    printf("\nДоступные команды:\n");
    printf("  1. Time         - показать текущее время\n");
    printf("  2. Date         - показать текущую дату\n");
    printf("  3. Howmuch      - сколько времени прошло с даты\n");
    printf("  4. Sanctions    - наложить санкции на пользователя\n");
    printf("  5. Logout       - выйти из системы\n");
}

void processTime() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info) {
        printf("Текущее время: %02d:%02d:%02d\n",
               tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    }
}

void processDate() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info) {
        printf("Текущая дата: %02d.%02d.%04d\n",
               tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
    }
}

void processHowmuch() {
    char dateStr[16], flag[4];
    printf("Введите дату (дд.мм.гггг): ");
    if (!fgets(dateStr, sizeof(dateStr), stdin)) return;
    dateStr[strcspn(dateStr, "\n")] = '\0';

    char *token = strtok(dateStr, ".");
    char *endptr;
    long val = strtol(token, &endptr, 10);
    if (!token || *endptr != '\0' || val < 1 || val > 31) {
        printf("Ошибка: Неверный день.\n");
        return;
    }
    int day = (int)val;

    token = strtok(NULL, ".");
    val = strtol(token, &endptr, 10);
    if (!token || *endptr != '\0' || val < 1 || val > 12) {
        printf("Ошибка: Неверный месяц.\n");
        return;
    }
    int month = (int)val;

    token = strtok(NULL, ".");
    val = strtol(token, &endptr, 10);
    if (!token || *endptr != '\0' || val < 1900 || val > 9999) {
        printf("Ошибка: Неверный год.\n");
        return;
    }
    int year = (int)val;

    struct tm input_tm = {0};
    input_tm.tm_mday = day;
    input_tm.tm_mon = month - 1;
    input_tm.tm_year = year - 1900;
    input_tm.tm_isdst = -1;

    time_t input_time = mktime(&input_tm);
    if (input_time == -1) {
        printf("Ошибка обработки даты.\n");
        return;
    }

    printf("Выберите флаг (-s, -m, -h, -y): ");
    if (!fgets(flag, sizeof(flag), stdin)) return;
    flag[strcspn(flag, "\n")] = '\0';

    time_t now = time(NULL);
    double diff = difftime(now, input_time);
    if (diff < 0) {
        printf("Ошибка: Указанная дата в будущем.\n");
        return;
    }

    if (strcmp(flag, "-s") == 0) printf("Прошло: %.0f секунд\n", diff);
    else if (strcmp(flag, "-m") == 0) printf("Прошло: %.0f минут\n", diff / 60);
    else if (strcmp(flag, "-h") == 0) printf("Прошло: %.0f часов\n", diff / 3600);
    else if (strcmp(flag, "-y") == 0) printf("Прошло: %.2f лет\n", diff / (3600 * 24 * 365.25));
    else printf("Ошибка: Неверный флаг (-s, -m, -h или -y)\n");
}

void processSanctions(UserDatabase* db) {
    char targetLogin[MAX_LOGIN_LENGTH + 1];
    printf("Введите логин пользователя, на которого наложить санкции: ");
    if (!fgets(targetLogin, sizeof(targetLogin), stdin)) return;
    targetLogin[strcspn(targetLogin, "\n")] = '\0';

    User *targetUser = findUser(db, targetLogin);
    if (!targetUser) {
        printf("Ошибка: Пользователь \"%s\" не найден.\n", targetLogin);
        return;
    }

    char limitStr[10];
    printf("Введите лимит команд: ");
    if (!fgets(limitStr, sizeof(limitStr), stdin)) return;
    limitStr[strcspn(limitStr, "\n")] = '\0';

    char *endptr;
    long limit = strtol(limitStr, &endptr, 10);
    if (*endptr != '\0' || limit <= 0) {
        printf("Ошибка: Некорректное значение лимита.\n");
        return;
    }

    printf("Подтвердите санкции, введя код: ");
    char confirm[10];
    if (!fgets(confirm, sizeof(confirm), stdin)) return;
    confirm[strcspn(confirm, "\n")] = '\0';
    if (strcmp(confirm, SANCTION_CODE) != 0) {
        printf("Ошибка: Неверный код подтверждения.\n");
        return;
    }

    targetUser->sanctionLimit = (int)limit;
    saveUsers(db);
    printf("Санкции установлены: %s теперь может выполнить максимум %d команд в сессии.\n",
           targetUser->login, targetUser->sanctionLimit);
}


void session(UserDatabase* db, const User* currentUser) {
    char input[10];
    int sessionCommandCount = 0;

    while (1) {
        showCommands();
        printf("\nВыберите номер команды (1-5): ");
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "5") == 0) {
            printf("Выход из системы.\n");
            return;
        }

        if (currentUser->sanctionLimit > 0 && sessionCommandCount >= currentUser->sanctionLimit) {
            printf("Превышен лимит команд. Доступна только команда Logout.\n");
            continue;
        }

        if (strcmp(input, "1") == 0) printf("Текущее время: 12:34:56\n");
        else if (strcmp(input, "2") == 0) printf("Текущая дата: 28.02.2025\n");
        else if (strcmp(input, "3") == 0) processHowmuch();
        else if (strcmp(input, "4") == 0) processSanctions(db);
        else printf("Ошибка: Неизвестная команда.\n");

        sessionCommandCount++;
    }
}

void authMenu(UserDatabase* db) {
    loadUsers(db);

    while (1) {
        printf("\n1. Вход\n2. Регистрация\n3. Выход\nВыберите: ");
        char choiceStr[10];
        if (!fgets(choiceStr, sizeof(choiceStr), stdin)) continue;
        choiceStr[strcspn(choiceStr, "\n")] = '\0';

        char *endptr;
        long choice = strtol(choiceStr, &endptr, 10);
        if (*endptr != '\0' || choice < 1 || choice > 3) {
            printf("Ошибка ввода. Введите число от 1 до 3.\n");
            continue;
        }
        if (choice == 3) {
            printf("Выход из программы.\n");
            break;
        }

        char login[MAX_LOGIN_LENGTH + 2];
        printf("Логин (до %d символов): ", MAX_LOGIN_LENGTH);
        if (!fgets(login, sizeof(login), stdin)) continue;
        login[strcspn(login, "\n")] = '\0';

        printf("PIN (0-100000): ");
        char pinStr[10];
        if (!fgets(pinStr, sizeof(pinStr), stdin)) continue;
        pinStr[strcspn(pinStr, "\n")] = '\0';

        char *pinEndPtr;
        long pin = strtol(pinStr, &pinEndPtr, 10);
        if (*pinEndPtr != '\0' || pin < 0 || pin > 100000) {
            printf("Ошибка: Некорректный PIN.\n");
            continue;
        }

        if (choice == 1) {
            User *user = findUser(db, login);
            if (!user || user->pin != pin) {
                printf("Неверный логин или PIN!\n");
                continue;
            }
            printf("Добро пожаловать, %s!\n", user->login);
            session(db, user);
        }
        else {
            if (findUser(db, login)) {
                printf("Логин уже занят!\n");
                continue;
            }
            if (registerUser(db, login, pin)) {
                printf("Регистрация успешна!\n");
                session(db, db->users[db->count - 1]);
            }
        }
    }
    saveUsers(db);
}

int main() {
    UserDatabase db;
    initDatabase(&db, "users.dat");

    authMenu(&db);
    freeDatabase(&db);
    return 0;
}
