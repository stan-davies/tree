#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
        struct stat st;
        stat(argv[1], &st);
        printf("%s gives %d which is... %d\n", argv[1], st.st_mode & S_IFMT, S_IFDIR);
        return 0;
}
