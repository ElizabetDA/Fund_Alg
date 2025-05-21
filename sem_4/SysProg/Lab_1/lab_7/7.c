#include "myls.h"

void get_perms(mode_t mode, char *perms) {
    snprintf(perms, 10, "%c%c%c%c%c%c%c%c%c",
        (mode & S_IRUSR) ? 'r' : '-',
        (mode & S_IWUSR) ? 'w' : '-',
        (mode & S_IXUSR) ? 'x' : '-',
        (mode & S_IRGRP) ? 'r' : '-',
        (mode & S_IWGRP) ? 'w' : '-',
        (mode & S_IXGRP) ? 'x' : '-',
        (mode & S_IROTH) ? 'r' : '-',
        (mode & S_IWOTH) ? 'w' : '-',
        (mode & S_IXOTH) ? 'x' : '-');
}

char get_ftype(mode_t mode) {
    if (S_ISDIR(mode)) return 'd';
    if (S_ISCHR(mode)) return 'c';
    if (S_ISBLK(mode)) return 'b';
    if (S_ISFIFO(mode)) return 'p';
    if (S_ISLNK(mode)) return 'l';
    if (S_ISSOCK(mode)) return 's';
    return '-';
}

void get_user_group(uid_t uid, gid_t gid, char **u, char **g) {
    struct passwd *pwd = getpwuid(uid);
    *u = pwd ? strdup(pwd->pw_name) : NULL;

    struct group *grp = getgrgid(gid);
    *g = grp ? strdup(grp->gr_name) : NULL;
}

void format_time(time_t t, char *buf) {
    struct tm *tm = localtime(&t);
    strftime(buf, 20, "%b %d %H:%M", tm);
}

void free_info(FileInfo *fi) {
    free(fi->user);
    free(fi->group);
    free(fi->name);
    free(fi->link);
}

void print_file(FileInfo *fi) {
    printf("%10lu %c%s %2lu %-8s %-8s %8ld %s %s",
        fi->ino,
        fi->type,
        fi->perms,
        fi->nlink,
        fi->user ? fi->user : "?",
        fi->group ? fi->group : "?",
        fi->size,
        fi->mtime,
        fi->name);

    if (fi->link) printf(" -> %s", fi->link);
    printf("\n");
}

void process_file(const char *path) {
    struct stat st;
    if (lstat(path, &st)) {
        fprintf(stderr, "ls: %s: %s\n", path, strerror(errno));
        return;
    }

    FileInfo fi = {0};
    fi.ino = st.st_ino;
    fi.type = get_ftype(st.st_mode);
    get_perms(st.st_mode, fi.perms);
    fi.nlink = st.st_nlink;
    get_user_group(st.st_uid, st.st_gid, &fi.user, &fi.group);
    fi.size = st.st_size;
    format_time(st.st_mtime, fi.mtime);
    fi.name = strdup(path);

    if (fi.type == 'l') {
        char link[PATH_MAX];
        ssize_t len = readlink(path, link, sizeof(link)-1);
        if (len > 0) {
            link[len] = '\0';
            fi.link = strdup(link);
        }
    }

    print_file(&fi);
    free_info(&fi);
}

void process_dir(const char *dir) {
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "ls: %s: %s\n", dir, strerror(errno));
        return;
    }

    struct dirent *ent;
    while ((ent = readdir(dp))) {
        char *path;
        if (asprintf(&path, "%s/%s", dir, ent->d_name) == -1) continue;

        process_file(path);
        free(path);
    }
    closedir(dp);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        process_dir(".");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        struct stat st;
        if (lstat(argv[i], &st)) {
            fprintf(stderr, "ls: %s: %s\n", argv[i], strerror(errno));
            continue;
        }

        S_ISDIR(st.st_mode) ? process_dir(argv[i]) : process_file(argv[i]);
    }

    return 0;
}