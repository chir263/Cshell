#include "pre_headers.h"

int i = 0;

char **get_args_(char *command)
{
    char **ret = init_string_arr(200);
    char *it = strtok(command, "|");
    while (it)
    {
        ret[i++] = it;
        it = strtok(NULL, "|");
    }
    ret[i] = NULL;
    return ret;
}

void piping(char *command)
{
    char **args = get_args_(command);
    int input[2], fd = 0;

    for (int j = 0; args[j]; j++)
    {
        pipe(input);
        pid_t pid = fork();

        if (pid == -1)
            perror("error");

        else if (!pid)
        {
            dup2(fd, STDIN_FILENO);

            if (args[j + 1])
                dup2(input[1], STDOUT_FILENO);

            close(input[0]);
            execute_command(args[j], 1);
            exit(1);
        }

        else
        {
            wait(NULL);
            close(input[1]);
            fd = input[0];
        }
    }
    i = 0;
    // printf("i am out\n");
}
