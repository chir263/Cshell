#include "pre_headers.h"

char *init_string(int size)
{
    char *str = (char *)malloc(sizeof(char) * size);
    return str;
}

char **init_string_arr(int size)
{
    char **str = (char **)malloc(sizeof(char *) * size);
    return str;
}

void trim(char *string)
{
    int len = strlen(string);
    int size = 0;
    for (int i = 0; i < len; i++)
    {
        switch (string[i])
        {
        case '\n':
        case ' ':
        case '\t':
            break;
        default:
            string[size++] = string[i];
        }
    }
    string[size] = '\0';
}
char **chunks;
void execute_command(char *command)
{
    // char *local_command = (char *)malloc(sizeof(char) * 1500);
    // strcpy(local_command, command);
    // trim(local_command);
    // printf("start -> %s\n", command);
    chunks = init_string_arr(500);
    char *local_command2 = (char *)malloc(sizeof(char) * 1500);
    strcpy(local_command2, command);
    // local_command = strtok(local_command, "\r\n\t ");

    if (local_command2)
    {
        // char chunks[1000][1024];
        int chunk_num = 0;
        // printf("%s\n", local_command2);
        local_command2 = strtok(local_command2, "\r\n\t ");
        while (local_command2)
        {
            // init_string(chunks[chunk_num], strlen(local_command) + 1);
            chunks[chunk_num] = (char *)malloc(100);
            strcpy(chunks[chunk_num++], local_command2);
            local_command2 = strtok(NULL, "\r\n\t ");
            // printf("%s\n", chunks[chunk_num - 1]);
        }

        // for (int i = 0; i < chunk_num; i++)
        // {
        //     printf("%s\n", chunks[i]);
        // }

        // trim(command);
        command = strtok(command, "\r\n\t ");
        if (command)
        {
            // printf("%s\n", command);
            int cl_size = strlen(chunks[chunk_num - 1]);
            chunks[chunk_num] = NULL;
            if (!strcmp(chunks[chunk_num - 1], "&"))
            {
                chunks[chunk_num - 1] = NULL;
                execute(chunks, chunk_num, BACK);
                // printf("back\n");
            }
            else if (chunks[chunk_num - 1][cl_size - 1] == '&')
            {
                chunks[chunk_num - 1][cl_size - 1] = '\0';
                execute(chunks, chunk_num, BACK);
                // printf("back\n");
            }
            else if (!strcmp(command, "pwd"))
                pwd();
            else if (!strcmp(command, "echo"))
                echo(command);
            else if (!strcmp(command, "cd"))
            {
                cd(command);

                // getcwd(curr_dir, sizeof(curr_dir));
                // printf("cd() -> %s\n", curr_dir);
            }
            else if (!strcmp(command, "ls"))
                ls(command);
            else if (!strcmp(command, "pinfo"))
                pinfo(command);
            else if (!strcmp(command, "history"))
                history(command);
            else if (!strcmp(command, "exit"))
            {
                // add_history();
                exit(0);
            }
            else if (!strcmp(command, "clear"))
            {
                printf("\e[1;1H\e[2J");
            }
            else if (!strcmp(command, "discover"))
            {
                discover(command);
            }
            else
                execute(chunks, chunk_num, FORE);
        }
        free(local_command2);
        // free(command);
    }
}