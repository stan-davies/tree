#ifndef GET_DIR_H
#define GET_DIR_H

int get_dir(char (*pth)[MPL], int shw, struct item (*lc_itms)[MFC], int *li_c) {
        DIR *dir = opendir(*pth);
        if (NULL == dir) {
                return FALSE;
        }

        int ret = FALSE;

// Scrap this split and just fill the main array.
// Sort it at the end, put directories before files and go alphabetically.
        *li_c = 0;
        struct item dirs[MFC / 4];
        int dirc = 0;
        struct item fils[MFC / 4 * 3];
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
                        dirs[dirc++] = curr;
                        if (dirc >= MFC / 4) {
                                goto exit;
                        }
                } else {
                        curr.is_dir = FALSE;
                        fils[filc++] = curr;
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

        int swaps;
        int sz_i;
        int sz_iS;

        struct item swp;

        for (;;) {
                swaps = FALSE;
                for (int i = 0; i < *li_c - 1; ++i) {
                        sz_i  = strcmp((*lc_itms)[i].name, "")
                                        - 100 * (*lc_itms)[i].is_dir;
                        sz_iS = strcmp((*lc_itms)[i + 1].name, "")
                                        - 100 * (*lc_itms)[i + 1].is_dir;

                        if (sz_iS < sz_i) {
                                swp = (*lc_itms)[i];
                                (*lc_itms)[i] = (*lc_itms)[i + 1];
                                (*lc_itms)[i + 1] = swp;
                                swaps = TRUE;
                        }
                }
                
                if (!swaps) {
                        break;
                }
        }
                        
                        

        ret = TRUE;
exit:
        closedir(dir);
        return ret;
}

#endif
