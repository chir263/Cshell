#ifndef __COMMANDS_H
#define __COMMANDS_H

char history_data[21][1024];
int history_num;

char print_promp[300];

typedef struct b_process_id
{
    char name[300];
    pid_t id;
} b_process_id;
int foreground_bool;
b_process_id task[2000];
int b_process;
char *init_string(int size);
char **init_string_arr(int size);
void execute_command(char *command);
void cd(char *command);
void pwd();
void echo(char *command);
void ls(char *command);
void pinfo(char *command);
void history_load();
void add_history();
void history(char *command);
void execute(char **chunks, int chunk_num, int type);
void discover(char *command);
int isdir(char *file);
b_process_id *create_process(pid_t id, char *name);

#endif