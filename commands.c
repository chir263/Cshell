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

// void trim(char *string)
// {
//     int len = strlen(string);
//     int size = 0;
//     for (int i = 0; i < len; i++)
//     {
//         switch (string[i])
//         {
//         case '\n':
//         case ' ':
//         case '\t':
//             break;
//         default:
//             string[size++] = string[i];
//         }
//     }
//     string[size] = '\0';
// }
char **chunks;
void execute_command(char *command, int type)
{
    char temp_command[1024];
    strcpy(temp_command, command);
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
        char comm2[1024];
        strcpy(comm2, command);

        if (strstr(command, "|") != NULL)
        {
            piping(command);
            return;
        }

        char *r1 = strstr(comm2, ">");
        char *r2 = strstr(comm2, "<");

        if (!((r1 == NULL) && (r2 == NULL)))
        {
            redirection(command);
            return;
        }

        command = strtok(command, "\r\n\t ");
        if (command)
        {
            // printf("%s\n", command);
            int cl_size = strlen(chunks[chunk_num - 1]);
            chunks[chunk_num] = NULL;
            if (!strcmp(chunks[chunk_num - 1], "&") && type)
            {
                chunks[chunk_num - 1] = NULL;
                execute(temp_command, chunks, chunk_num, BACK);
                // printf("back\n");
            }
            else if (chunks[chunk_num - 1][cl_size - 1] == '&' && type)
            {
                chunks[chunk_num - 1][cl_size - 1] = '\0';
                execute(temp_command, chunks, chunk_num, BACK);
                // printf("back\n");
            }
            else if (!strcmp(command, "pwd") && type && !ctrlc_fore)
                pwd();
            else if (!strcmp(command, "echo") && type && !ctrlc_fore)
                echo(command);
            else if (!strcmp(command, "cd") && type && !ctrlc_fore)
            {
                cd(command);

                // getcwd(curr_dir, sizeof(curr_dir));
                // printf("cd() -> %s\n", curr_dir);
            }
            else if (!strcmp(command, "ls") && type && !ctrlc_fore)
                ls(command);
            else if (!strcmp(command, "pinfo") && !ctrlc_fore)
                pinfo(command);
            else if (!strcmp(command, "history") && type && !ctrlc_fore)
                history(command);
            else if (!strcmp(command, "exit") && type && !ctrlc_fore)
            {
                // add_history();
                exit(0);
            }
            else if (!strcmp(command, "clear") && type && !ctrlc_fore)
            {
                printf("\e[1;1H\e[2J");
            }
            else if (!strcmp(command, "discover") && !ctrlc_fore)
            {
                discover(command);
            }
            else if (!strcmp(command, "jobs") && !ctrlc_fore)
            {
                print_jobs(command);
            }
            else if (!strcmp(command, "sig") && !ctrlc_fore)
            {
                sig(command);
            }
            else if (!strcmp(command, "fg") && !ctrlc_fore)
            {
                fg(command);
            }
            else if (!strcmp(command, "bg") && !ctrlc_fore)
            {
                bg(command);
            }
            else if (!ctrlc_fore)
            {
                // printf("%d \n", ctrlc_fore);
                execute(temp_command, chunks, chunk_num, FORE);
            }
        }
        free(local_command2);
        // free(command);
    }
}