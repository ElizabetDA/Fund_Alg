#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <limits.h>

typedef struct {
	ino_t ino;
	char type;
	char perms[10];   // Права доступа (rwx-строка)
	nlink_t nlink;   // Количество жестких ссылок
	char *user;
	char *group;
	off_t size;
	char mtime[20];    // Время модификации
 	char *name;
	char *link;      // Цель символической ссылки
} FileInfo;

#endif // COMMON_H