#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#define MAX_LOGIN_LENGTH 6
#define MAX_USERS 100
#define SANCTION_CODE "12345"
#define USER_DB_FILE "users.dat"

typedef struct {
    char login[MAX_LOGIN_LENGTH + 1];
    int pin;
    int sanctionLimit;
} User;

User *users[MAX_USERS] = {NULL};
int userCount = 0;

void loadUsers();
void saveUsers();
User* findUser(const char *login);
int registerUser(const char *login, int pin);
void processSanctions();
void session(const User *currentUser);

void loadUsers() {
    FILE *file = fopen(USER_DB_FILE, "rb");
    if (!file) return;

    while (userCount < MAX_USERS) {
        User *user = malloc(sizeof(User));
        if (fread(user, sizeof(User), 1, file) != 1) {
            free(user);
            break;
        }
        users[userCount++] = user;
    }
    fclose(file);
}

void saveUsers() {
    FILE *file = fopen(USER_DB_FILE, "wb");
    if (!file) return;

    for (int i = 0; i < userCount; i++) {
        fwrite(users[i], sizeof(User), 1, file);
    }
    fclose(file);
}

int getIntegerInput(const char *prompt, int min, int max) {
    while (1) {
        printf("%s", prompt);
        char input[32];
        if (!fgets(input, sizeof(input), stdin)) continue;

        char *endptr;
        errno = 0;
        long value = strtol(input, &endptr, 10);

        if (endptr == input || *endptr != '\n' || errno == ERANGE) {
            printf("Некорректный ввод. Попробуйте снова.\n");
            continue;
        }

        if (value >= min && value <= max) return (int)value;
        printf("Значение должно быть от %d до %d\n", min, max);
    }
}

User* findUser(const char *login) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i]->login, login) == 0) {
            return users[i];
        }
    }
    return NULL;
}

int registerUser(const char *login, int pin) {
    if (userCount >= MAX_USERS) {
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

    users[userCount++] = newUser;
    saveUsers();
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
    } else {
        printf("Ошибка: невозможно получить время.\n");
    }
}

void processDate() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info) {
        printf("Текущая дата: %02d.%02d.%04d\n",
               tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
    } else {
        printf("Ошибка: невозможно получить дату.\n");
    }
}

void processSanctions() {
    char targetLogin[MAX_LOGIN_LENGTH + 1];
    printf("Введите логин пользователя, на которого наложить санкции: ");
    if (!fgets(targetLogin, sizeof(targetLogin), stdin)) {
        return;
    }
    targetLogin[strcspn(targetLogin, "\n")] = '\0';
    User *targetUser = findUser(targetLogin);
    if (targetUser == NULL) {
        printf("Ошибка: Пользователь \"%s\" не найден.\n", targetLogin);
        return;
    }

    char limitStr[10];
    printf("Введите лимит команд для пользователя %s: ", targetUser->login);
    if (!fgets(limitStr, sizeof(limitStr), stdin)) {
        return;
    }
    limitStr[strcspn(limitStr, "\n")] = '\0';

    char *endptr;
    errno = 0;
    long limit = strtol(limitStr, &endptr, 10);
    if (endptr == limitStr || *endptr != '\0' || errno == ERANGE || limit <= 0) {
        printf("Ошибка: Некорректное значение лимита.\n");
        return;
    }

    printf("Подтвердите санкции, введя код: ");
    char confirm[10];
    if (!fgets(confirm, sizeof(confirm), stdin)) {
        return;
    }
    confirm[strcspn(confirm, "\n")] = '\0';
    if (strcmp(confirm, SANCTION_CODE) != 0) {
        printf("Ошибка: Неверный код подтверждения.\n");
        return;
    }

    targetUser->sanctionLimit = (int)limit;
    printf("Санкции установлены: пользователь %s теперь может выполнить максимум %d команд в сессии.\n",
           targetUser->login, targetUser->sanctionLimit);
}

int parseDateComponent(const char *str, int min, int max) {
    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0' || errno == ERANGE || val < min || val > max) {
        return -1;
    }
    return (int)val;
}

void processHowmuch() {
    char dateStr[16], flag[4];
    int day, month, year;

    printf("Введите дату (дд.мм.гггг): ");
    if (!fgets(dateStr, sizeof(dateStr), stdin)) return;
    dateStr[strcspn(dateStr, "\n")] = '\0';

    char *rest = dateStr;
    char *token = strtok(rest, ".");
    if (!token || (day = parseDateComponent(token, 1, 31)) == -1) {
        printf("Ошибка дня.\n");
        return;
    }

    token = strtok(NULL, ".");
    if (!token || (month = parseDateComponent(token, 1, 12)) == -1) {
        printf("Ошибка месяца.\n");
        return;
    }

    token = strtok(NULL, ".");
    if (!token || (year = parseDateComponent(token, 1900, 9999)) == -1) {
        printf("Ошибка года.\n");
        return;
    }

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

    if (strcmp(flag, "-s") == 0) {
        printf("Прошло: %.0f секунд\n", diff);
    } else if (strcmp(flag, "-m") == 0) {
        printf("Прошло: %.0f минут\n", diff / 60);
    } else if (strcmp(flag, "-h") == 0) {
        printf("Прошло: %.0f часов\n", diff / 3600);
    } else if (strcmp(flag, "-y") == 0) {
        printf("Прошло: %.2f лет\n", diff / (3600 * 24 * 365.25));
    } else {
        printf("Ошибка: Неверный флаг (-s, -m, -h или -y)\n");
    }
}

void session(const User *currentUser) {
    char input[10];
    int sessionCommandCount = 0;
    while (1) {
        showCommands();
        printf("Выберите номер команды: ");
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

        int validCommand = 0;
        if (strcmp(input, "1") == 0) {
            processTime();
            validCommand = 1;
        } else if (strcmp(input, "2") == 0) {
            processDate();
            validCommand = 1;
        } else if (strcmp(input, "3") == 0) {
            processHowmuch();
            validCommand = 1;
        } else if (strcmp(input, "4") == 0) {
            processSanctions();
        } else {
            printf("Ошибка: Неизвестная команда.\n");
        }

        if (validCommand) {
            sessionCommandCount++;
        }
    }
}

void authMenu() {
    loadUsers();

    while (1) {
        printf("\n1. Вход\n2. Регистрация\n3. Выход\nВыберите: ");

        int choice = getIntegerInput("", 1, 3);
        if (choice == 3) break;

        char login[MAX_LOGIN_LENGTH + 2];
        printf("Логин (до %d символов): ", MAX_LOGIN_LENGTH);
        if (!fgets(login, sizeof(login), stdin)) continue;
        login[strcspn(login, "\n")] = '\0';

        if (strlen(login) > MAX_LOGIN_LENGTH) {
            printf("Логин слишком длинный!\n");
            continue;
        }

        int pin = getIntegerInput("PIN (0-100000): ", 0, 100000);

        if (choice == 1) {
            User *user = findUser(login);
            if (!user || user->pin != pin) {
                printf("Неверный логин или PIN!\n");
                continue;
            }
            printf("Добро пожаловать, %s!\n", user->login);
            session(user);
        }
        else if (choice == 2) {
            if (findUser(login)) {
                printf("Логин уже занят!\n");
                continue;
            }
            if (registerUser(login, pin)) {
                printf("Регистрация успешна!\n");
                session(users[userCount-1]);
            }
        }
    }

    saveUsers();
}

void freeMemory() {
    for (int i = 0; i < userCount; i++) {
        free(users[i]);
    }
}

int main() {
    authMenu();
    freeMemory();
    return 0;
}