#include <stdio.h>
#include <string.h>
#include "pre_headers.h"

struct termios orig_termios;

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    // die("tcsetattr");
}

int isfile(char *path)
{
    struct stat st;
    stat(path, &st);
    return S_ISREG(st.st_mode);
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    // die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    // die("tcsetattr");
}

char *get_input()
{
    setbuf(stdout, NULL);
    enableRawMode();
    // printf("Prompt>");
    char *inp = malloc(sizeof(char) * 1020);
    memset(inp, '\0', 100);
    int pt = 0;
    char c;
    if (autocomplete_print)
    {
        autocomplete_print = 0;
        // if (autocomplete_nxt[strlen(autocomplete_nxt) - 1] == '\n')
        //     autocomplete_nxt[strlen(autocomplete_nxt) - 1] == '\0';

        int x = strlen(autocomplete_nxt) - 1;
        while (autocomplete_nxt[x] == '\n')
            autocomplete_nxt[x] = '\0';

        printf("%s", autocomplete_nxt);
        strcat(inp, autocomplete_nxt);
        pt = strlen(inp);
    }
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                inp[pt++] = c;
                printf("\n");
                break;
            }
            else if (c == 4)
            {
                exit(0);
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                  // printf("\rarrow key: %s", buf);
                }
            }
            else if (c == 9)
            { // TAB character
                // printf("cdlskv");
                int space = inp[pt - 1] == ' ';
                if (inp[pt - 1] == '/' || !pt || inp[pt - 1] == ' ')
                    continue;
                char temstr[100];
                char temstr1[100];
                int sz = 0;
                int stop_auto = pt - 1;
                int slash = 1;
                int check = 0;
                for (int x = pt - 1; x >= 0; x--, stop_auto--)
                {
                    if (inp[x] == '/' || (inp[x] == ' '))
                    {
                        slash += x;
                        break;
                    }
                    temstr[sz++] = inp[x];
                }
                for (int x = sz - 1; x >= 0; x--)
                    temstr1[sz - x - 1] = temstr[x];
                temstr1[sz] = '\0';
                strcpy(autocomplete_dir, curr_dir);
                strcat(autocomplete_dir, "/");
                int temp_size_auto = 0;
                int ptr_auto = pt;
                for (int x = pt - 1; inp[x] != ' ' && x >= 0; x--, ptr_auto--)
                    ;

                for (temp_size_auto = strlen(autocomplete_dir); ptr_auto < stop_auto; ptr_auto++)
                    autocomplete_dir[temp_size_auto++] = inp[ptr_auto];
                autocomplete_dir[temp_size_auto] = '\0';

                char *text = autocomplete(temstr1, autocomplete_dir);
                // printf("%s\n", text);

                // if (!strcmp(text, "") && strcmp(temstr1, "") && temstr1 != NULL)
                if (autocomplete_print)
                {
                    inp[pt++] = '\n';
                    printf("\n");
                    strcpy(autocomplete_str, temstr1);
                    break;
                }
                else
                {
                    for (int x = 0; x < strlen(text); x++)
                    {
                        inp[pt++] = text[x];
                        printf("%c", text[x]);
                    }
                    strcat(autocomplete_dir, "/");
                    strcat(autocomplete_dir, inp + slash);
                    // strcat(autocomplete_dir, text);
                    // printf("  %d %s\n", slash, autocomplete_dir);
                    if (isdir(autocomplete_dir))
                    {

                        inp[pt++] = '/';
                        printf("%c", '/');
                    }
                    else if (isfile(autocomplete_dir))
                    {
                        inp[pt++] = ' ';
                        printf("%c", ' ');
                    }
                    inp[pt] = '\0';

                    // int stp =
                    // for (int x = pt - 1; inp[x] != ' ' && x >= 0; x--)
                    //     temstr[sz++] = inp[x];
                }

                // printf("%s", text);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }
    if (autocomplete_print)
    {
        strcpy(autocomplete_nxt, inp);
    }
    disableRawMode();
    return inp;
}

// int main()
// {
//     char *inp = get_input();
//     printf("\n%s\n", inp);
//     return 0;
// }