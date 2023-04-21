#include "pre_headers.h"

char process_status[1024];

b_process_id temp_b_process[200];

int cmp(const void *p, const void *q)
{
    return strcmp(((b_process_id *)p)->name,
                  ((b_process_id *)q)->name);
}

void paste()
{
    for (int i = 0; i < b_process; i++)
    {
        temp_b_process[i].id = task[i].id;
        temp_b_process[i].num = task[i].num;
        strcpy(temp_b_process[i].name, task[i].name);
    }
    qsort(temp_b_process, b_process, sizeof(b_process_id), cmp);
}

void print_jobs(char *command)
{
    paste();
    command = strtok(NULL, "\r\n\t ");
    if (command)
    {
        if (!strcmp(command, "-s"))
        {
            for (int i = 0; i < b_process; i++)
            {
                sprintf(process_status, "/proc/%d/stat", temp_b_process[i].id);
                FILE *status_file = fopen(process_status, "r");
                if (status_file == NULL)
                {
                    printf("error: process doesn't exist\n");
                    return;
                }
                char status_;
                fscanf(status_file,
                       "%*d %*s %c %*s %*d %*s %*s %*d %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                       "%*s %*s %*s %*s %*s %*d",
                       &status_);
                if (status_ == 'T')
                {
                    for (int i = 0; i < b_process; i++)
                        printf("[%d] Stopped %s [%d]\n", temp_b_process[i].num, temp_b_process[i].name, temp_b_process[i].id);
                }
            }
        }
        else
        {
            for (int i = 0; i < b_process; i++)
                printf("[%d] Running %s [%d]\n", temp_b_process[i].num, temp_b_process[i].name, temp_b_process[i].id);
        }
    }
    else
    {
        for (int i = 0; i < b_process; i++)
        {
            sprintf(process_status, "/proc/%d/stat", temp_b_process[i].id);
            FILE *status_file = fopen(process_status, "r");
            if (status_file == NULL)
            {
                printf("error: process doesn't exist\n");
                return;
            }
            char status_;
            fscanf(status_file,
                   "%*d %*s %c %*s %*d %*s %*s %*d %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                   "%*s %*s %*s %*s %*s %*d",
                   &status_);
            if (status_ == 'T')
                printf("[%d] Stopped %s [%d]\n", temp_b_process[i].num, temp_b_process[i].name, temp_b_process[i].id);
            else
                printf("[%d] Running %s [%d]\n", temp_b_process[i].num, temp_b_process[i].name, temp_b_process[i].id);
        }
    }
}

void sig(char *command)
{
    char *x1 = strtok(NULL, "\r\n\t ");
    char *x2 = strtok(NULL, "\r\n\t ");
    if (x1 && x2)
    {
        int job_num = atoi(x1);
        int signal = atoi(x2);
        for (int i = 0; i < b_process; i++)
        {
            if (task[i].num == job_num)
            {
                kill(task[i].id, signal);
                return;
            }
        }
    }
    printf("job doesn't exist\n");
}

void fg(char *command)
{
    char *x1 = strtok(NULL, "\r\n\t ");
    if (x1)
    {
        int job_num = atoi(x1);
        // printf("in %d %d\n", job_num, b_process);
        for (int i = 0; i < b_process; i++)
        {
            if (task[i].num == job_num)
            {
                time_t start_t, end_t;
                start_t = time(NULL);
                // printf("matched\n");
                char t_name[1024];
                pid_t pid = task[i].id;
                foreground_bool = 1;
                FG_ID = pid;
                strcpy(t_name, task[i].name);

                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(STDIN_FILENO, pid);
                kill(pid, SIGCONT);
                int status;
                waitpid(pid, &status, WUNTRACED);
                // printf("Dfs\n");
                tcsetpgrp(STDIN_FILENO, getpgrp());
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);

                foreground_bool = 0;
                // wait(NULL);
                end_t = time(NULL);
                int total_t = (int)(end_t - start_t);

                char time[10];
                if (total_t >= 1)
                    sprintf(time, "took %ds", total_t);
                else
                    strcpy(time, "");
                // shellPrompt(username, HOME, curr_dir, system_name, time);print_promp
                strcpy(print_promp, time);

                // printf("detected\n");

                if (!WIFSTOPPED(status))
                {
                    for (int j = i; j < b_process - 1; j++)
                    {
                        strcpy(task[j].name, task[j + 1].name);
                        task[j].id = task[j + 1].id;
                        task[j].num = task[j + 1].num;
                    }
                    b_process--;
                    // printf("fore\n");

                    FG_ID = pid;
                }
                else
                {
                    t_name[strlen(t_name) - 1] = '\0';
                    printf("%s with pid %d stopped\n", t_name, pid);
                }
                return;
            }
        }
    }
    printf("job doesn't exist\n");
}

void bg(char *command)
{
    char *x1 = strtok(NULL, "\r\n\t ");
    if (x1)
    {
        int job_num = atoi(x1);
        for (int i = 0; i < b_process; i++)
        {
            if (task[i].num == job_num)
            {
                pid_t pid = task[i].id;
                sprintf(process_status, "/proc/%d/stat", pid);
                FILE *status_file = fopen(process_status, "r");
                if (status_file == NULL)
                {
                    printf("error: process doesn't exist\n");
                    return;
                }
                char status_;
                fscanf(status_file,
                       "%*d %*s %c %*s %*d %*s %*s %*d %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                       "%*s %*s %*s %*s %*s %*d",
                       &status_);
                if (status_ == 'T')
                    kill(pid, SIGCONT);
                return;
            }
        }
        printf("job doesn't exist\n");
    }
}