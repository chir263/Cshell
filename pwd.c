#include "pre_headers.h"

char *curr_dir_;
void pwd()
{
    curr_dir_ = init_string(2048);
    curr_dir_ = getcwd(NULL, 0);
    // printf("%s\n", curr_dir_);
    if (curr_dir_)
        printf("%s\n", curr_dir_);
    else
    {
        perror("error");
        exit(1);
    }
    free(curr_dir_);
    return;
}