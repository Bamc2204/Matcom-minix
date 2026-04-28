#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define PATH_MAX 1024

void print_tree(const char *path, int level);

int main(int argc, char *argv[])
{
    const char *start_path;
    if (argc < 2)
        start_path = ".";
    else
        start_path = argv[1];

    print_tree(start_path, 0);
    return 0;
}

void print_tree(const char *path, int level)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char fullpath[PATH_MAX];

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "tree: cannot open directory '%s': %s\n",
                path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &st) == -1) {
            fprintf(stderr, "tree: cannot stat '%s': %s\n",
                    fullpath, strerror(errno));
            continue;
        }

        for (int i = 0; i < level; i++)
            printf("    ");
        printf("%s\n", entry->d_name);

        if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
            print_tree(fullpath, level + 1);
        }
    }
    closedir(dir);
}
