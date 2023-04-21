#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "shellPrompt.h"
#include "commands.h"
#include <time.h>
#include <termios.h>
#include <ctype.h>
// #include <conio.h>

// #include <curses.h>
// #include <conio.h>
#define ll long long

#define BACK 12
#define FORE 11
int ctrlc_fore;
char *HOME;
char *username;
char system_name[1024];
char *curr_dir;
pid_t TERMINAL_ID, FG_ID;
char *prev_dir;
char *his_file_name;
int autocomplete_print;
char autocomplete_str[1024];
char autocomplete_dir[1024];
char autocomplete_nxt[1024];