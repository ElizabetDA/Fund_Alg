#include "app.h"

void add_file_to_dir(DirEntry **dirs, const char *dir_path, const char *file_name) {
    DirEntry *entry = *dirs;
    DirEntry *prev = NULL;
    while (entry != NULL) {
        if (strcmp(entry->path, dir_path) == 0) break;
        prev = entry;
        entry = entry->next;
    }
    if (!entry) {
        entry = (DirEntry*)malloc(sizeof(DirEntry));
        if (!entry) { perror("malloc"); exit(EXIT_FAILURE); }
        entry->path = strdup(dir_path);
        entry->files = NULL;
        entry->next = *dirs;
        *dirs = entry;
    }
    FileNode *file = entry->files;
    while (file) {
        if (strcmp(file->name, file_name) == 0) return;
        file = file->next;
    }
    FileNode *new_file = (FileNode*)malloc(sizeof(FileNode));
    if (!new_file) { perror("malloc"); exit(EXIT_FAILURE); }
    new_file->name = strdup(file_name);
    new_file->next = entry->files;
    entry->files = new_file;
}

void process_input(DirEntry **dirs, const char *input) {
	// Создаем две независимые копии исходного пути
	char *path_dir = strdup(input);
	if (!path_dir) { perror("strdup"); exit(EXIT_FAILURE); }
	char *path_file = strdup(input);
	if (!path_file) { perror("strdup"); free(path_dir); exit(EXIT_FAILURE); }

	// Обрабатываем каталог и файл раздельно
	char *dir_path = dirname(path_dir);
	char *file_name = basename(path_file);

	add_file_to_dir(dirs, dir_path, file_name);

	free(path_dir);
	free(path_file);
}

void free_dirs(DirEntry *dirs) {
    while (dirs) {
        DirEntry *next = dirs->next;
        FileNode *file = dirs->files;
        while (file) {
            FileNode *next_file = file->next;
            free(file->name);
            free(file);
            file = next_file;
        }
        free(dirs->path);
        free(dirs);
        dirs = next;
    }
}

char* generate_output(DirEntry *dirs) {
    size_t buf_size = 1024;
    char *buffer = (char*)malloc(buf_size);
    if (!buffer) { perror("malloc"); exit(EXIT_FAILURE); }
    buffer[0] = '\0';
    size_t len = 0;
    for (DirEntry *d = dirs; d; d = d->next) {
        char dir_line[2048] = {0};
        snprintf(dir_line, sizeof(dir_line), "%s: ", d->path);
        size_t dir_line_len = strlen(dir_line);
        if (len + dir_line_len + 1 > buf_size) {
            buf_size *= 2;
            buffer = (char*)realloc(buffer, buf_size);
            if (!buffer) { perror("realloc"); exit(EXIT_FAILURE); }
        }
        strcat(buffer + len, dir_line);
        len += dir_line_len;
        FileNode *file = d->files;
        while (file) {
            size_t name_len = strlen(file->name);
            if (len + name_len + 2 > buf_size) {
                buf_size *= 2;
                buffer = (char*)realloc(buffer, buf_size);
                if (!buffer) { perror("realloc"); exit(EXIT_FAILURE); }
            }
            strcat(buffer + len, file->name);
            len += name_len;
            file = file->next;
            if (file) {
                strcat(buffer + len, ", ");
                len += 2;
            }
        }
        strcat(buffer + len, "\n");
        len += 1;
    }
    return buffer;
}

int main() {
    const char *in_fifo = "/tmp/server_input.fifo";
    const char *out_fifo = "/tmp/server_output.fifo";

    mkfifo(in_fifo, 0666);
    mkfifo(out_fifo, 0666);

    while (1) {
        int in_fd = open(in_fifo, O_RDONLY);
        if (in_fd == -1) { perror("open in_fifo"); exit(EXIT_FAILURE); }

        DirEntry *dirs = NULL;
        char buf[1024];
        ssize_t bytes_read;
        char *line = NULL;
        size_t line_len = 0;

        while ((bytes_read = read(in_fd, buf, sizeof(buf))) > 0) {
            for (ssize_t i = 0; i < bytes_read; ++i) {
                if (buf[i] == '\n') {
                    if (line_len > 0) {
                        line = realloc(line, line_len + 1);
                        if (!line) { perror("realloc"); exit(EXIT_FAILURE); }
                        line[line_len] = '\0';
                        if (line[0] == '/') process_input(&dirs, line);
                        free(line);
                        line = NULL;
                        line_len = 0;
                    }
                } else {
                    line = realloc(line, line_len + 1);
                    if (!line) { perror("realloc"); exit(EXIT_FAILURE); }
                    line[line_len++] = buf[i];
                }
            }
        }
        if (line_len > 0) {
            line[line_len] = '\0';
            if (line[0] == '/') process_input(&dirs, line);
            free(line);
        }
        close(in_fd);

        char *output = generate_output(dirs);
        int out_fd = open(out_fifo, O_WRONLY);
        if (out_fd == -1) { perror("open out_fifo"); free(output); free_dirs(dirs); exit(EXIT_FAILURE); }
        write(out_fd, output, strlen(output));
        close(out_fd);

        free(output);
        free_dirs(dirs);
    }

    unlink(in_fifo);
    unlink(out_fifo);
    return 0;
}