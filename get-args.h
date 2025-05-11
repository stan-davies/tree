#ifndef READ_ARGS_H
#define READ_ARGS_H

/*
 *   INPUT  : `argc` and `argv` as given in `main`. Pointer to path that may be
 *            specified. Pointer to maximum recursion depth that may be
 *            specified. Pointer to boolean for whether or not to show hidden
 *            files that may be specified.
 *   OUTPUT : Success/failure.
 *
 *   DESC   : Converts given arguments to data to run with.
 */
int get_args(int argc, char **argv, char (*path)[MPL], int *depth, int *show_all) {
        int c;

        while ((c = getopt(argc, argv, ":f:d:al")) != -1) {
                switch (c) {
                case 'f':
                        memcpy(*path, optarg, MPL);
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
                case 'l':
                        *depth = 0;
                        break;
                case ':':
                        printf("No value given for option '%c'.\n", optopt);
                case '?':
                default:
                        printf("Invalid option given at '%c'.\n", optopt);
                        return FALSE;
                }
        }

        return TRUE;
}

#endif
