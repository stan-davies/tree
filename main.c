#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define TRUE  1
#define FALSE 0
#define NULL  (void *)0

#define MAX_PATH_LENGTH 32
#define DOT_CHAR 46
#define MAX_DEPTH 3

int tree_dir(char *path, int depth) {
        if (depth > MAX_DEPTH) {
                return FALSE;
        }

        DIR *dir = opendir(path);
        if (NULL == dir) {
                return FALSE;
        }

        struct dirent *dp;
        struct stat st;
        while ((dp = readdir(dir)) != NULL) {
                if (DOT_CHAR == dp->d_name[0]) {
                        continue;
                }

                stat(dp->d_name, &st);

                for (int i = 0; i < depth; ++i) {
                        printf("│   "); 
                }
                printf("├── %s", dp->d_name); 

                if ((st.st_mode & S_IFMT) == S_IFDIR) {
                        printf("/\n");
                        if (!tree_dir(dp->d_name, depth + 1)) {
                                (void)closedir(dir);
                                return FALSE;
                        }
                } else {
                        printf("\n");
                }
        }

        (void)closedir(dir);

        return TRUE;
}

int main (int argc, char **argv) {
        char *path = calloc(MAX_PATH_LENGTH, sizeof(char));
        path[0] = DOT_CHAR;
        printf(".\n");
        if (!tree_dir(path, 0)) {
                printf("Could not print tree\n");
        }

	return 0;
}
