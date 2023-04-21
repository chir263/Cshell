#include "pre_headers.h"

void shellPrompt(char *user, char *root, char *cwd,
                 char *hostname, char *extra)
{

    // curr_dir= getcwd(, sizeof(curr_dir));
    // printf("%s\n", curr_dir);
    if (cwd)
    {
        char *temp_dir;
        temp_dir = strstr(cwd, root);
        (temp_dir) ? printf("<\033[1;32m%s@%s\033[0m:\033[1;34m~%s %s\033[0m> ", user, hostname, temp_dir + strlen(root), extra)
                   : printf("<\033[1;32m%s@%s\033[0m:\033[1;34m%s %s\033[0m> ", user, hostname, cwd, extra);
    }
    else
    {
        perror("Current Working Directory Error");
        exit(1);
    }
}