#include "pre_headers.h"
// #include <readline/readline.h>
// #include "conio.c"
// #include "curses.h"
int number_of_commands = 0;
int tem_commands = 0;
char **get_commands(char *input)
{
    char *command = strtok(input, ";");
    char **array_of_commands = init_string_arr(200);
    for (number_of_commands = 0; command;)
    {
        // printf("reading commands %d - %s\n", number_of_commands, command);
        // char *command2 = init_string(200);
        array_of_commands[number_of_commands] = init_string(200);
        strcpy(array_of_commands[number_of_commands++], command);
        // strcpy(command2, command);
        // command2 = strtok(command2, "&");
        // while (command2)
        // {
        //     printf("%s\n", command2);
        //     command2 = strtok(NULL, ";");
        // }

        command = strtok(NULL, ";");
    }
    // array_of_commands[number_of_commands] = NULL;
    return array_of_commands;
}

char **get_commands_and(char *input)
{
    int count = 0;
    for (int i = 0; i < strlen(input); i++)
        if (input[i] == '&')
            count++;
    char *command = strtok(input, "&");
    char **array_of_commands = init_string_arr(200);
    for (tem_commands = 0; command; tem_commands++)
    {
        // printf("reading commands %d - %s\n", number_of_commands, command);
        array_of_commands[tem_commands] = init_string(200);
        strcpy(array_of_commands[tem_commands], command);
        if (count)
        {
            strcat(array_of_commands[tem_commands], "&");
            count--;
        }
        // printf("%s   %d   %c\n", command, strlen(command), command + strlen(command));
        // if (command[strlen(command) + 1] == '&')
        // {
        //     printf("%s%c", command, '&');
        // }
        command = strtok(NULL, "&");
    }
    return array_of_commands;
}

void getHOME()
{
    pid_t p_id = getpid();
    TERMINAL_ID = p_id;
    char exec[50];
    sprintf(exec, "/proc/%d/exe", p_id);
    int ind = readlink(exec, HOME, 1000);
    HOME[ind] = '\0';
    size_t len = strlen(HOME);
    for (int i = len - 1; i >= 0; i--)
    {
        if (HOME[i] == '/')
        {
            HOME[i] = '\0';
            break;
        }
    }
}

void init_promp_()
{
    username = init_string(1024);
    // curr_dir = init_string(1024);
    HOME = init_string(1024);
    // username = getenv("USER");
    getlogin_r(username, sizeof(username));
    curr_dir = init_string(1024);
    curr_dir = getcwd(NULL, 0);
    // printf("%s\n", curr_dir);
    gethostname(system_name, 1024);
    getHOME();
}

void function()
{
    int status;
    char cmd_name[1024];
    pid_t pid = waitpid(-1, &status, WNOHANG);
    int flag = 0;
    if (pid > 0)
    {
        for (int i = 0; i < b_process; i++)
        {
            if (pid == task[i].id)
            {

                strcpy(cmd_name, task[i].name);
                for (int j = i; j < b_process - 1; j++)
                {
                    task[i].id = task[i + 1].id;
                    task[i].num = task[i + 1].num;
                    strcpy(task[i].name, task[i + 1].name);
                }
                b_process--;
                flag = 1;
                break;
            }
        }
        if (WEXITSTATUS(status) == 0 && WIFEXITED(status) && flag)
            printf("\n%s with PID %d exited normally\n", cmd_name, pid);
        else if (flag)
            printf("\n%s with PID %d failed to exit normally\n", cmd_name, pid);
        // if (f)
        // printf("\n");
        if (!foreground_bool)
            shellPrompt(username, HOME, curr_dir, system_name, print_promp);
        fflush(stdout);
        // fprintf(stdin, "\n");
    }

    return;
}

void ctrlc()
{
    pid_t p = getpid();
    ctrlc_fore = 1;
    // printf("i am here\n");
    if (p != TERMINAL_ID)
        return;
    if (FG_ID != -1)
    {
        kill(FG_ID, SIGINT);
    }
    signal(SIGINT, ctrlc);
    printf("\n");
}

void ctrlz()
{
    pid_t p = getpid();
    // ctrlc_fore = 1;
    // printf("i am here\n");
    if (p == TERMINAL_ID)
        return;
    if (FG_ID != -1)
    {
        kill(FG_ID, SIGTSTP);
    }
    signal(SIGINT, ctrlz);
    printf("\n");
}

void loop_shell()
{
    char *input_line;
    while (1)
    {
        ctrlc_fore = 0;
        shellPrompt(username, HOME, curr_dir, system_name, print_promp);
        FG_ID = -1;
        signal(SIGCHLD, function);
        signal(SIGINT, ctrlc);
        // signal(SIGTSTP, ctrlz);

        strcpy(print_promp, "");
        // print_promp = "";
        input_line = get_input();
        // printf("%s", input_line);
        if (strcmp(history_data[(history_num - 1)], strcat(input_line, "\n")))
        {
            add_history(input_line);
            // strcpy(history_data[(history_num++) % 20], command);
        }
        if (autocomplete_print)
        {
            print_autocomplete(autocomplete_str, autocomplete_dir);
        }
        else if (strcmp(input_line, "\n") != 0)
        {

            char **list_of_commands = get_commands(input_line);

            list_of_commands[number_of_commands - 1][strlen(list_of_commands[number_of_commands - 1]) - 1] = '\0';
            for (int i = 0; i < number_of_commands; i++)
            {
                char asd[1024];
                strcpy(asd, list_of_commands[i]);
                tem_commands = 0;
                char **list_of_commands_and = get_commands_and(asd);
                for (int j = 0; j < tem_commands; j++)
                {
                    // printf("%s %d\n", list_of_commands_and[j], number_of_commands);
                    char agsd[1024];
                    strcpy(agsd, list_of_commands_and[j]);
                    execute_command(agsd, 1);
                }
                // if (list_of_commands[i] != NULL || strcmp(list_of_commands[i], ""))
                // printf("begin -> %s\n", list_of_commands[i]);
                // execute_command(asd);
            }
            // execute_command("echo asdafadsdfsdgd\n");
            // char *str = "echo asdcafasfds  ds fds fdsf\n";
            // str = strtok(str, "\"\r\n\t ");
            // echo(str);
            // echo("  echo asdcafasfasfsaf ds f dsfg dsg g\n");
        }
        // execute_command("echo asdafa");
    }
}

int main()
{
    b_process = 0;
    // autocomplete_nxt_flag = 0;
    strcpy(print_promp, "");
    printf("\e[1;1H\e[2J");
    // signal(SIGINT, SIG_IGN);
    // signal(SIGTSTP, SIG_IGN);
    init_promp_();
    // printf("%s\n", autocomplete("ec"));
    FG_ID = -1;
    TERMINAL_ID = getpid();
    history_load();
    foreground_bool = 0;
    prev_dir = init_string(2048);
    strcpy(prev_dir, "..");
    ctrlc_fore = 0;
    loop_shell();

    return 0;
}