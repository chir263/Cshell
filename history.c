#include "pre_headers.h"

int min(int a, int b)
{
    return (a < b) ? a : b;
}

void print_history(int num)
{
    int n = min(num, min(history_num, 20));
    for (int i = 0 + history_num - n; i < history_num; i++)
    {
        for (int x = strlen(history_data[i % 20]) - 1; x >= 0; x--)
        {
            if (history_data[i % 20][x] == '\n')
                history_data[i % 20][x] = '\0';
            else
                break;
        }
        printf("%d  %s\n", i + 1 - (history_num - n), history_data[i % 20]);
    }
    // printf("%d %s\n", n, (n == 1) ? "command" : "commands");
}

void history_load()
{
    his_file_name = init_string(1024);
    sprintf(his_file_name, "%s/%s", HOME, "history.txt");
    FILE *file = fopen(his_file_name, "r");
    if (!file)
        perror("unable to acces history.txt");
    char line[128];
    while (fgets(line, sizeof(line), file))
    {
        if (line != NULL && strcmp(line, "") && strcmp(line, "\n"))
            strcpy(history_data[(history_num++) % 20], line);
    }
    fclose(file);
}

void add_history(char *command)
{
    FILE *file = fopen(his_file_name, "w");
    if (history_num == 20)
    {
        for (int i = 1; i < history_num; i++)
        {
            strcpy(history_data[i - 1], history_data[i]);
            fprintf(file, "%s\n", history_data[i]);
        }
        strcpy(history_data[19], command);
        fprintf(file, "%s\n", command);
    }
    else
    {
        strcpy(history_data[(history_num++)], command);
        for (int i = 0; i < history_num; i++)
        {
            fprintf(file, "%s\n", history_data[i]);
        }
    }
    fclose(file);
}

void history(char *command)
{
    command = strtok(NULL, "\r\n\t ");
    (command)
        ? print_history(atoi(command))
        : print_history(10);
}