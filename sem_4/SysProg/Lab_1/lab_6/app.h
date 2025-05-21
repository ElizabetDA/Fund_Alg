#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>

typedef struct FileNode {
	char *name;
	struct FileNode *next;
} FileNode;

typedef struct DirEntry {
	char *path;
	FileNode *files;
	struct DirEntry *next;
} DirEntry;

#endif // COMMON_H