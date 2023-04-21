#include "pre_headers.h"

void execute(char **chunks, int chunk_num, int type)
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
            waitpid(pid, &status, WUNTRACED);
            foreground_bool = 0;
            // wait(NULL);
            end_t = time(NULL);
            int total_t = (int)(end_t - start_t);
            // printf("%lf %lld %lld\n", (end_t - start_t), start_t, end_t);
            // for (int i = 0; i < chunk_num; i++)
            // {
            //     free(chunks[i]);
            // }
            // free(chunks);
            // time add
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
            strcpy(task[b_process++].name, chunks[0]);
            printf("[%d] %d\n", b_process, pid);
        }
    }
}
