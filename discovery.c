#include "pre_headers.h"

int flag = 0, f_ = 0, d_ = 0;

char tilda[10] = "";

int isdir(char *file)
{
    struct stat sfile;
    return (stat(file, &sfile) == 0 && S_ISDIR(sfile.st_mode)) ? 1 : 0;
}

int dot(char *file)
{
    return !strcmp(file, "..") || !strcmp(file, ".");
}

void util_discover(char *cwd, char *dir, char *search)
{
    // printf("got in %s\n", dir);
    struct dirent **list_files;
    char *out_path = init_string(strlen(cwd) + 1050);
    sprintf(out_path, "%s/%s", cwd, dir);
    int num_of_files = scandir(out_path, &list_files, NULL, alphasort);
    if (num_of_files < 0)
    {
        // perror("error");
        return;
    }

    for (int i = 0; i < num_of_files; i++)
    {
        if (f_ && d_)
        {
            // printf("searching in %s with %s\n", out_path, (list_files[i]->d_name));
            if (!strcmp((list_files[i]->d_name), search) || !strcmp(search, ""))
                printf("%s%s/%s\n", tilda, dir, list_files[i]->d_name);
        }
        else if (f_)
        {
            // printf("fsdf\n");
            if ((!strcmp(search, "") || !strcmp((list_files[i]->d_name), search)) && !dot(list_files[i]->d_name))
            {
                char vcx[1024];
                sprintf(vcx, "%s/%s", out_path, list_files[i]->d_name);
                if (!isdir(vcx))
                    printf("%s%s/%s\n", tilda, dir, list_files[i]->d_name);
            }
        }
        else if (d_)
        {
            if ((!strcmp(search, "") || !strcmp((list_files[i]->d_name), search)) && !dot(list_files[i]->d_name))
            {
                char vcx[1024];
                sprintf(vcx, "%s/%s", out_path, list_files[i]->d_name);
                if (isdir(vcx))
                    printf("%s%s/%s\n", tilda, dir, list_files[i]->d_name);
            }
        }
        if (!dot(list_files[i]->d_name))
        {
            char temp[1024];
            sprintf(temp, "%s/%s", dir, list_files[i]->d_name);
            util_discover(cwd, temp, search);
        }
        // (list_files[i]->d_name);
        free(list_files[i]);
    }
    free(out_path);
    free(list_files);
}

void discover(char *command)
{
    command = strtok(NULL, "\r\n\t ");
    flag = 0, f_ = 0, d_ = 0;
    char dir_name[1024] = ".";
    char file_name[1024] = "";
    char *cwd;
    cwd = getcwd(NULL, 0);

    while (command)
    {
        if (!strcmp(command, "-f"))
        {
            f_ = 1;
            flag = 1;
        }
        else if (!strcmp(command, "-d"))
        {
            d_ = 1;
            flag = 1;
        }
        else if (command != NULL)
        {
            if (command[0] == '"')
            {
                if (strcmp(command, "\"\""))
                {
                    int n = strlen(command) - 1;
                    for (int i = 1; i < n; i++)
                    {
                        file_name[i - 1] = command[i];
                    }
                    file_name[n - 1] = '\0';
                }
            }
            else if (!strcmp(command, "~"))
            {
                // printf("i am here\n");
                cwd = (char *)realloc(cwd, 1024);
                strcpy(cwd, HOME);
                // printf("i am here1\n");
                strcpy(tilda, "~");
                strcpy(dir_name, "");
                // printf("i am here2\n");
            }
            else
            {
                // printf("i am not here\n");

                strcpy(dir_name, command);
            }
        }
        command = strtok(NULL, "\r\n\t ");
    }
    if (!flag)
        f_ = 1, d_ = 1;
    // printf("%s\n%s\n%s\n%d\n%d\n", cwd, dir_name, file_name, d_, f_);
    util_discover(cwd, dir_name, file_name);
    strcpy(tilda, "");
}