#include "pre_headers.h"

pid_t p_id;
char *process_status;
char *exe;
FILE *file;
char *return_info;
char *status, *memory;

char *get_info_from_file(int n)
{
    file = fopen(process_status, "r");
    char info_[256];
    return_info = init_string(256);
    for (int i = 0; fgets(info_, sizeof(info_), file); i++)
        if (i == n - 1)
            strcpy(return_info, info_);
    return return_info;
}

void setpid(char *command)
{
    (command) ? (p_id = atoi(command)) : (p_id = getpid());
    // printf("pid : %d\n", p_id);
}

void pinfo(char *command)
{
    command = strtok(NULL, "\r\n\t ");
    process_status = init_string(50);
    exe = init_string(50);
    setpid(command);
    sprintf(process_status, "/proc/%d/stat", p_id);
    sprintf(exe, "/proc/%d/exe", p_id);
    char *exe_path = init_string(2000);
    int ex_code = readlink(exe, exe_path, 1024);
    FILE *status_file = fopen(process_status, "r");
    if (status_file == NULL)
    {
        printf("error: process doesn't exist\n");
        return;
    }
    int pid, pgrp, tpgid;
    char status_;
    int memory_;
    fscanf(status_file,
           "%d %*s %c %*s %d %*s %*s %d %*s %*s %*s %*s %*s %*s %*s %*s %*s "
           "%*s %*s %*s %*s %*s %d",
           &pid, &status_, &pgrp, &tpgid, &memory_);
    printf("pid : %d\nprocess Status : %c%c\nmemory : %d\n", pid, status_,
           pgrp != tpgid ? ' ' : '+', memory_);
    if (ex_code < 0)
        printf("executable path : None\n");
    else
    {
        exe_path[ex_code] = '\0';
        char *temp = strstr(exe_path, HOME);
        (temp) ? printf("executable path : ~%s\n", temp + strlen(HOME))
               : printf("executable path : %s\n", exe_path);
    }
    free(status);
    free(process_status);
    free(memory);
}