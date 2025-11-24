#ifndef GET_DIR_H
#define GET_DIR_H

void place(
        struct item     itm     ,
        struct item   **arr     ,
        int            *arr_c
) {
        int p = 0;

        for (; p < *arr_c; ++p) {
                if (strcmp(itm.name, (*arr)[p].name) < 0) {
                        break;
                }
        }

        for (int i = *arr_c; i > p; --i) {
                (*arr)[i] = (*arr)[i - 1];
        }
        
        (*arr)[p] = itm;
        (*arr_c)++;
}

int get_dir(char (*pth)[MPL], int shw, struct item (*lc_itms)[MFC], int *li_c) {
        DIR *dir = opendir(*pth);
        if (NULL == dir) {
                return FALSE;
        }

        int ret = FALSE;

        *li_c = 0;
        struct item *dirs = calloc(MFC / 4, sizeof(struct item));
        int dirc = 0;
        struct item *fils = calloc(MFC / 4 * 3, sizeof(struct item));
        int filc = 0;

        char rel_pth[MPL];

        struct dirent *dp;
        struct stat it_st;
        struct item curr;

        while ((dp = readdir(dir)) != NULL) {
                if (strcmp(dp->d_name, CUR_DIR) == 0 
                                || strcmp(dp->d_name, PRE_DIR) == 0) {
                        continue;
                }

                if (!shw && DOT_CHR == dp->d_name[0]) {
                        continue;
                }

                sprintf(rel_pth, "%s/%s", *pth, dp->d_name);
                if (stat(rel_pth, &it_st) < 0) {
                        goto exit;
                }

                strcpy(curr.name, dp->d_name);
                if ((it_st.st_mode & S_IFMT) == S_IFDIR) {
                        curr.is_dir = TRUE;
                        place(curr, &dirs, &dirc);
                        if (dirc >= MFC / 4) {
                                goto exit;
                        }
                } else {
                        curr.is_dir = FALSE;
                        place(curr, &fils, &filc);
                        if (filc >= MFC / 4 * 3) {
                                goto exit;
                        }
                }

                if (dirc + filc >= MFC) {
                        goto exit;
                }
        }

        *li_c = dirc + filc;
        for (int i = 0; i < *li_c; ++i) {
                if (i < dirc) {
                        (*lc_itms)[i] = dirs[i]; 
                } else {
                        (*lc_itms)[i] = fils[i - dirc];
                }
        }

        ret = TRUE;
exit:
        closedir(dir);
        return ret;

        free(dirs);
        dirs = NULL;
        free(fils);
        fils = NULL;
}

#endif
