#include "pre_headers.h"

#define OUTDIR_DBL 2
#define OUTDIR_SGL 1

int args_num = 0;

char **get_args(char *command)
{
    char **arguments = init_string_arr(256);

    while (command)
    {
        arguments[args_num] = init_string(100);
        strcpy(arguments[args_num++], command);
        command = strtok(NULL, "\r\n\t ");
    }
    arguments[args_num] = NULL;
    return arguments;
}

void free_args(char **args)
{
    for (int j = 0; j < args_num; j++)
        free(args[j]);
    free(args);
}

void redirection(char *command)
{
    int std_in_new, std_out_new;
    char *output[2], *input[2];
    int out_dir_type = 0;
    char *f_out = NULL;
    char *f_in = NULL;

    int status;

    std_out_new = dup(STDOUT_FILENO);
    std_in_new = dup(STDIN_FILENO);

    // rm out.txt; cat < history.txt | wc > out.txt; cat out.txt
    int in_dir = strstr(command, "<") != NULL;

    if (strstr(command, ">"))
        out_dir_type = OUTDIR_SGL;
    else if (strstr(command, ">>"))
        out_dir_type = OUTDIR_DBL;

    output[0] = command;

    if (out_dir_type)
    {
        char *temp_str;
        temp_str = strtok(command, ">");
        output[0] = temp_str;
        temp_str = strtok(NULL, ">");
        output[1] = temp_str;
        f_out = strtok(output[1], "\r\n\t ");
    }

    char temp_output[1024];
    strcpy(temp_output, output[0]);
    input[0] = temp_output;

    if (in_dir)
    {
        input[0] = strtok(input[0], "<");
        input[1] = strtok(NULL, "<");
        if (!input[1])
        {
            perror("error: name for input\n");
            return;
        }
        else
        {
            f_in = strtok(input[1], "\r\n\t ");
            struct stat f;
            if (!(stat(f_in, &f) == 0) || S_ISDIR(f.st_mode))
            {
                printf("file does not exist\n");
                return;
            }
        }
    }

    input[0] = strtok(input[0], "\r\n\t ");
    char temp_inp[1024];
    strcpy(temp_inp, input[0]);
    // char **args = get_args("temp_inp");

    if (out_dir_type && !f_out)
    {
        perror("error: name for output\n");
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("error");
        return;
    }
    else if (pid)
    {
        while (wait(&status) != pid)
            // nothing
            ;
    }
    else
    {
        int fd;
        if (in_dir)
        {
            fd = open(f_in, O_RDONLY);
            if (fd < 0)
            {
                perror("error in input redirection");
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (out_dir_type)
        {
            if (out_dir_type == OUTDIR_DBL)
                fd = open(f_out, O_CREAT | O_APPEND | O_WRONLY, 0644);
            else if (out_dir_type == OUTDIR_SGL)
                fd = open(f_out, O_CREAT | O_TRUNC | O_WRONLY, 0644);

            if (fd == -1)
            {
                perror("error in output redirection");
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execute_command(temp_inp, 1);

        dup2(std_out_new, STDOUT_FILENO);
        close(std_out_new);

        dup2(std_in_new, STDIN_FILENO);
        close(std_in_new);
    }

    // free_args(args);
}
