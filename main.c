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

#define MPL 256 // max path length
#define MFC 32  // max file count

#define DOT_CHR 46
#define CUR_DIR ".\0"
#define PRE_DIR "..\0"

struct item {
        char name[MPL];
        int  is_dir;
};

#include "get-dir.h"
#include "print-tree.h"
#include "get-args.h"

int main(int argc, char **argv) {
        char path[MPL]; 
        strcpy(path, CUR_DIR);
        int depth = 3;
        int show_all = FALSE;

        if (!get_args(argc, argv, &path, &depth, &show_all)) {
                printf("Could not get args.\n");
                return 0;
        }

        printf("%s\n", path);
        if (!tree(&path, depth, 0, show_all, 0)) {
                printf("Could not print tree.\n");
        }

	return 0;
}
