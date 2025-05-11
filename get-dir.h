#ifndef GET_DIR_H
#define GET_DIR_H

int get_dir(char (*pth)[MPL], int shw_a, struct item (*lc_itms)[MFC], int *li_c) {
        DIR *dir = opendir(*pth);
        if (NULL == dir) {
                return FALSE;
        }

        *li_c = 0;

        char rel_pth[MPL];

        struct dirent *dp;
        struct stat it_st;
        struct item curr;

        while ((dp = readdir(dir)) != NULL) {
                if (strcmp(dp->d_name, CUR_DIR) == 0 
                                || strcmp(dp->d_name, PRE_DIR) == 0) {
                        continue;
                }

                if (!shw_a && DOT_CHR == dp->d_name[0]) {
                        continue;
                }

                sprintf(rel_pth, "%s/%s", *pth, dp->d_name);
                if (stat(rel_pth, &it_st) < 0) {
                        closedir(dir);
                        return FALSE;
                }

                strcpy(curr.name, dp->d_name);
                curr.is_dir = (it_st.st_mode & S_IFMT) == S_IFDIR;

                (*lc_itms)[*li_c] = curr;
                (*li_c)++;

                if (*li_c >= MFC) {
                        closedir(dir);
                        return TRUE;
                }
        }

        closedir(dir);
        return TRUE;
}

#endif
