#include "pre_headers.h"

void execute(char *temp_command, char **chunks, int chunk_num, int type)
{
    // printf("into it\n");
    // for (int i = 0; i < chunk_num; i++)
    //     printf("%s ", chunks[i]);
    // printf("\n");
    pid_t pid = fork();
    time_t start_t, end_t;
    start_t = time(NULL);
    if (pid == -1)
    {
        perror("error");
        return;
    }
    else if (!pid)
    {
        // if (!chunks)
        //     printf("safafgagadsg\n");

        // printf("%s\n", chunks[0]);
        // int x = execvp(chunks[0], chunks);
        setpgid(0, 0);
        if (type == BACK)
        {
            setpgrp();
        }
        if (execvp(chunks[0], chunks) == -1)
            perror("error");
        exit(0);
    }
    else
    {
        if (type == FORE)
        {
            int status;
            foreground_bool = 1;
            FG_ID = pid;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, pid);

            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            foreground_bool = 0;
            // wait(NULL);
            end_t = time(NULL);
            int total_t = (int)(end_t - start_t);

            if (WIFSTOPPED(status))
            {
                task[b_process].id = pid;
                if (b_process)
                    task[b_process].num = task[b_process - 1].num + 1;
                else
                    task[b_process].num = 1;
                strcpy(task[b_process++].name, temp_command);

                if (task[b_process - 1].name[strlen(task[b_process - 1].name) - 1] == '\n')
                    task[b_process - 1].name[strlen(task[b_process - 1].name) - 1] = '\0';

                printf("%s with pid %d stopped\n", task[b_process - 1].name, pid);
            }
            char time[10];
            if (total_t >= 1)
                sprintf(time, "took %ds", total_t);
            else
                strcpy(time, "");
            // shellPrompt(username, HOME, curr_dir, system_name, time);print_promp
            strcpy(print_promp, time);
        }
        else if (type == BACK)
        {
            task[b_process].id = pid;
            if (b_process)
                task[b_process].num = task[b_process - 1].num + 1;
            else
                task[b_process].num = 1;
            strcpy(task[b_process].name, temp_command);
            printf("[%d] %d\n", task[b_process++].num, pid);
        }
    }
}
