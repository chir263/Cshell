#include "pre_headers.h"

void cd(char *command)
{
    command = strtok(NULL, "\r\n\t ");

    // printf("%s %s\n", command, HOME);
    char *newpath = init_string(1024);
    char *cwd;
    cwd = getcwd(NULL, 0);
    if (!command)
    {
        command = "~";
    }
    else if (!strcmp(command, "-"))
    {
        // printf("%s\n", prev_dir);
        strcpy(command, prev_dir);
    }
    char *cc = strcpy(prev_dir, cwd);
    // printf("%s\ncwd- > %s\n", cc, prev_dir);

    if (command[0] == '~')
    {

        // strcpy(newpath, cwd);
        // strcat(newpath, command + 1);
        if (0 > chdir(HOME))
            perror("error");
        if (prev_dir != NULL)
        {
            // printf("sfs %s\n", prev_dir);

            strcpy(prev_dir, command);
            // printf("sfs %s\n", prev_dir);
        }
        // printf("%s\n", HOME);
        strcpy(curr_dir, HOME);
        return;
    }
    else if (!strcmp(command, "/"))
    {
        strcpy(newpath, "/");
    }
    else
    {
        strcpy(newpath, command);
    }
    // printf("new--%s\n", newpath);
    if (chdir(newpath) < 0)
    {
        perror("error");
    }
    // else if (prev_dir != NULL)
    // {
    //     // printf("sfs %s\n", prev_dir);

    //     strcpy(prev_dir, command);
    //     // printf("sfs %s\n", prev_dir);
    // }

    cwd = getcwd(NULL, 0);
    // printf("update- > %s\n", cwd);
    // printf("%s\n", chunks[1]);
    curr_dir = (char *)realloc(curr_dir, 1024);
    strcpy(curr_dir, cwd);
}