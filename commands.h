#ifndef __COMMANDS_H
#define __COMMANDS_H

char history_data[21][1024];
int history_num;

char print_promp[300];

typedef struct b_process_id
{
    int num;
    char name[300];
    pid_t id;
} b_process_id;
int foreground_bool;
b_process_id task[2000];

int b_process;
char *init_string(int size);
char **init_string_arr(int size);
void redirection(char *command);
void execute_command(char *command, int type);
void cd(char *command);
void pwd();
void echo(char *command);
void ls(char *command);
void pinfo(char *command);
void history_load();
void add_history();
void history(char *command);
void execute(char *temp_command, char **chunks, int chunk_num, int type);
void discover(char *command);
int isdir(char *file);
void piping(char *command);
void print_jobs(char *command);
void sig(char *command);
void fg(char *command);
void bg(char *command);
char *autocomplete(char *name, char *curr);
void print_autocomplete(char *name, char *curr);
void enableRawMode();
void disableRawMode();
char *get_input();

b_process_id *create_process(pid_t id, char *name);

#endif