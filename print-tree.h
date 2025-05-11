#ifndef PRINT_TREE_H
#define PRINT_TREE_H


/*
 *   INPUT  : Pointer to character buffer containing the path of the directory
 *            to output. Max depth that may be read to (from '.' or what was
 *            given in -f). Current depth (from...). Whether or not to show
 *            hidden files. Boolean array containing indentation data.
 *   OUTPUT : Success.
 *
 *   DESC   : Outputs given directory as a tree. Works recursively, outputting
 *            subdirectories by calling this function again. For these calls,
 *            `ind` is updated to ensure correct indentation. In this array,
 *            1 -> | and 0 -> blank.
 */       
int tree(char (*pth)[MPL], int max_dpt, int dpt, int shw, int ind) {
        if (dpt > max_dpt) {
                return TRUE;
        }

        int ret = FALSE;

        struct item lc_itms[MFC];
        int li_c;
        char sub_pth[MPL];
        int sub_ind;

        if (!get_dir(pth, shw, &lc_itms, &li_c)) {
                printf("Could not access local files at '%s'\n", *pth);
                goto exit;
        }

        for (int i = 0; i < li_c; ++i) {
                for (int j = dpt - 1; j >= 0; --j) {
                        if ((ind & (1 << j)) >> j) {
                                printf("│   "); 
                        } else {
                                printf("    ");
                        }
                }

                if (li_c - 1 == i) {
                        printf("└── ");
                } else {
                        printf("├── "); 
                }

                printf("%s", lc_itms[i].name); 

                if (lc_itms[i].is_dir) {
                        printf("/\n");
                        sprintf(sub_pth, "%s/%s", *pth, lc_itms[i].name);
                        sub_ind = (ind << 1) + !(li_c - 1 == i);
                        if (!tree(&sub_pth, max_dpt, dpt + 1, shw, sub_ind)) {
                                goto exit;
                        }
                } else {
                        printf("\n");
                }
        }

        ret = TRUE;
exit:
//        free(sub_pth);
//        free(lc_itms);
        return ret;
}

#endif
