#include "pre_headers.h"
struct dirent **list_files;

int startsWith(const char *pre, const char *str)
{
    if (strlen(pre) > strlen(str))
        return 0;
    for (int i = 0; i < strlen(pre); i++)
        if (pre[i] != str[i])
            return 0;
    return 1;
}

char *autocomplete(char *name, char *curr)
{
    if (name == NULL || !strcmp(name, ""))
        return "";

    int num_of_files = scandir(curr, &list_files, NULL, alphasort);
    char **files = init_string_arr(100);
    int xx = 0;
    for (int i = 0; i < num_of_files; i++)
        if (startsWith(name, list_files[i]->d_name) && strcmp(name, ""))
            files[xx++] = list_files[i]->d_name;

    int size = strlen(name), size1 = 0;
    char *temp = init_string(100);
    while (1)
    {
        char temp_ = '\0';
        int flag = 1;
        for (int i = 0; i < xx; i++)
        {
            if (temp_ == '\0')
                temp_ = files[i][size];
            else
            {
                if (temp_ != files[i][size])
                {
                    // printf("%s %c %s %c", files[i - 1], temp_, files[i], files[i][size]);
                    flag = 0;
                    break;
                }
            }
        }
        if (temp_ == '\0')
            break;
        if (flag)
            temp[size1++] = files[0][size++];
        else
            break;
    }

    temp[size1] = '\0';
    // printf("%s %d %d\n", temp, strlen(temp), size);
    // exit(0);
    if (xx > 1 && !strcmp(temp, ""))
        autocomplete_print = 1;
    return temp;
    // return "";
}

void print_autocomplete(char *name, char *curr)
{
    int num_of_files = scandir(curr, &list_files, NULL, alphasort);
    int xx = 0;
    for (int i = 0; i < num_of_files; i++)
        if (startsWith(name, list_files[i]->d_name) && strcmp(name, ""))
        {
            char temp_file[1024];
            sprintf(temp_file, "%s/%s", curr, list_files[i]->d_name);
            if (isdir(temp_file))
                printf("%s/\t", list_files[i]->d_name);
            else
                printf("%s\t", list_files[i]->d_name);
        }
    printf("\n");
}