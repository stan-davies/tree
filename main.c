#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define TRUE  1
#define FALSE 0

#define MAX_PATH_LENGTH 256
#define MAX_FILE_COUNT  16

#define DOT_CHR 46
#define CUR_DIR ".\0"
#define PRE_DIR "..\0"

struct item {
        char name[MAX_PATH_LENGTH];
        int  is_dir;
};


int get_dir(char (*path)[MAX_PATH_LENGTH], int show_all, struct item (*lc_itms)[MAX_FILE_COUNT], int *item_c) {
        DIR *dir = opendir(*path);
        if (NULL == dir) {
                return FALSE;
        }

        *item_c = 0;

        char rel_path[MAX_PATH_LENGTH];

        struct dirent *dp;
        struct stat it_st;
        struct item curr;

        while ((dp = readdir(dir)) != NULL) {
                if (strcmp(dp->d_name, CUR_DIR) == 0 
                                || strcmp(dp->d_name, PRE_DIR) == 0) {
                        continue;
                }

                if (!show_all && DOT_CHR == dp->d_name[0]) {
                        continue;
                }

                sprintf(rel_path, "%s/%s", *path, dp->d_name);
                if (stat(rel_path, &it_st) < 0) {
                        closedir(dir);
                        return FALSE;
                }

                strcpy(curr.name, dp->d_name);
                curr.is_dir = (it_st.st_mode & S_IFMT) == S_IFDIR;

                (*lc_itms)[*item_c] = curr;
                (*item_c)++;

                if (*item_c >= MAX_FILE_COUNT) {
                        printf("Too many files!\n");
                        closedir(dir);
                        return FALSE;
                }
        }

        closedir(dir);
        return TRUE;
}

int tree_dir(char (*path)[MAX_PATH_LENGTH], int max_depth, int show_all, int depth, int indents) {
        if (depth > max_depth) {
                return TRUE;
        }
        
        struct item local_items[MAX_FILE_COUNT];
        int item_c;

        if (!get_dir(path, show_all, &local_items, &item_c)) {
                printf("Could not access local files at '%s'\n", *path);
                return FALSE;
        }

        char rel_path[MAX_PATH_LENGTH];

        for (int i = 0; i < item_c; ++i) {
                for (int j = 0; j < depth; ++j) {
                        if ((indents & i) != 0) {
                                printf("│   "); 
                        } else {
                                printf("    ");
                        }
                }

                if (item_c - 1 == i) {
                        printf("└── ");
                } else {
                        printf("├── "); 
                }

                printf("%s", local_items[i].name); 

                if (local_items[i].is_dir) {
                        printf("/\n");
                        sprintf(rel_path, "%s/%s", *path, local_items[i].name);
                        int child_indents = (indents + 1) + !(item_c - 1 == i);
                        if (!tree_dir(&rel_path, max_depth, show_all, depth + 1, child_indents)) {
                                return FALSE;
                        }
                } else {
                        printf("\n");
                }
        }

        return TRUE;
}

int get_args(int argc, char **argv, char (*path)[MAX_PATH_LENGTH], int *depth, int *show_all) {
        int c;

        while ((c = getopt(argc, argv, "f:d:a")) != -1) {
                switch (c) {
                case 'f':
                        memcpy(*path, optarg, MAX_PATH_LENGTH);
                        break;
                case 'd':
                        *depth = atoi(optarg);
                        if (depth <= 0) {
                                printf("Invalid depth given.\n");
                                return FALSE;
                        }
                        break;
                case 'a':
                        *show_all = TRUE;
                        break;
                default:
                        printf("Invalid argument given.\n");
                        return FALSE;
                }
        }

        return TRUE;
}

int main(int argc, char **argv) {
        char path[MAX_PATH_LENGTH]; 
        strcpy(path, CUR_DIR);
        int depth = 3;
        int show_all = FALSE;

        if (!get_args(argc, argv, &path, &depth, &show_all)) {
                printf("Could not get args.\n");
                return 0;
        }

        printf("%s\n", path);
        if (!tree_dir(&path, depth, show_all, 0, 1)) {
                printf("Could not print tree.\n");
        }

	return 0;
}
