#include "pre_headers.h"

int l_ = 0, a_ = 0;
struct dirent **list_files;
struct stat sfile;

char ff_name[1024];

// int isdir(char *file)
// {
//     struct stat sfile;
//     return (stat(file, &sfile) == 0 && S_ISDIR(sfile.st_mode)) ? 1 : 0;
// }
int block = 0;
void l_info(char *directory, char *filename)
{
    char *temp_file = init_string(strlen(directory) + strlen(filename) + 15);
    char *tt = init_string(50);
    strftime(tt, 50, "%b  %d %H:%M", localtime(&sfile.st_mtime));
    sprintf(temp_file, "%s/%s", directory, filename);
    if (stat(temp_file, &sfile) == -1)
        perror("Error occured");
    char info[50] = "";
    strcat(info, (stat(temp_file, &sfile) == 0 && S_ISDIR(sfile.st_mode)) ? "d" : "-");
    strcat(info, (sfile.st_mode & S_IRUSR) ? "r" : "-");
    strcat(info, (sfile.st_mode & S_IWUSR) ? "w" : "-");
    strcat(info, (sfile.st_mode & S_IXUSR) ? "x" : "-");
    strcat(info, (sfile.st_mode & S_IRGRP) ? "r" : "-");
    strcat(info, (sfile.st_mode & S_IWGRP) ? "w" : "-");
    strcat(info, (sfile.st_mode & S_IXGRP) ? "x" : "-");
    strcat(info, (sfile.st_mode & S_IROTH) ? "r" : "-");
    strcat(info, (sfile.st_mode & S_IWOTH) ? "w" : "-");
    strcat(info, (sfile.st_mode & S_IXOTH) ? "x" : "-");
    char dinfo[1024], xinfo[1024];
    sprintf(dinfo, "%s\t%ld\t%s\t%s\t%ld\t%s\t\033[1;34m%s\033[0m\n", info, sfile.st_nlink, getpwuid(sfile.st_uid)->pw_name, getgrgid(sfile.st_uid)->gr_name, sfile.st_size, tt, filename);
    sprintf(xinfo, "%s\t%ld\t%s\t%s\t%ld\t%s\t\033[1;32m%s\033[0m\n", info, sfile.st_nlink, getpwuid(sfile.st_uid)->pw_name, getgrgid(sfile.st_uid)->gr_name, sfile.st_size, tt, filename);
    if (info[0] == 'd')
        printf("%s", dinfo);
    else if (info[3] == 'x')
        printf("%s", xinfo);
    else
        printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%s\n", info, sfile.st_nlink, getpwuid(sfile.st_uid)->pw_name, getgrgid(sfile.st_uid)->gr_name, sfile.st_size, tt, filename);
    free(temp_file);
}

void util_ls(char *file_path)
{
    if (!isdir(file_path))
    {
        printf("%s\n", ff_name);
        return;
    }
    int num_of_files = scandir(file_path, &list_files, NULL, alphasort);
    if (num_of_files < 0)
    {
        perror("error");
        return;
    }

    if (l_)
    {
        for (int i = 0; i < num_of_files; i++)
        {
            char abs_path[1024];
            sprintf(abs_path, "%s/%s", file_path, list_files[i]->d_name);
            struct stat st;
            stat(abs_path, &st);
            if (a_ || (!a_ && !((list_files[i]->d_name)[0] == '.')))
                block += (st.st_blocks >> 1);
        }
        printf("total - %d\n", block);
        block = 0;
        for (int i = 0; i < num_of_files; i++)
        {
            if (a_ || (!a_ && !((list_files[i]->d_name)[0] == '.')))
                l_info(file_path, list_files[i]->d_name);
            free(list_files[i]);
        }
    }

    else
    {

        // printf("n -> %s\n", file_path);
        for (int i = 0; i < num_of_files; i++)
        {
            if (a_ || (!a_ && !((list_files[i]->d_name)[0] == '.')))
            {
                // if ()
                char abs_path[1024];
                sprintf(abs_path, "%s/%s", file_path, list_files[i]->d_name);
                if (isdir(abs_path))
                {
                    printf("\033[1;34m%s\033[0m\t", list_files[i]->d_name);
                }
                else if (stat(abs_path, &sfile) >= 0 && (sfile.st_mode & S_IXUSR))
                {
                    printf("\033[1;32m%s\033[0m\t", list_files[i]->d_name);
                }
                else
                {
                    printf("%s\t", list_files[i]->d_name);
                }
                free(list_files[i]);
            }
        }
        printf("\n");
    }
    free(list_files);
}

void ls(char *command)
{
    command = strtok(NULL, "\r\n\t ");
    l_ = 0, a_ = 0;
    // printf("%s\n", (command) ? "YES" : "NO");
    if (command)
    {
        int ex_cmd = 0;
        int in_cmd = 0;
        while (command)
        {
            block = 0;
            int size = strlen(command);
            if (!strcmp(command, "-a"))
            {
                a_ = 1;
                ex_cmd = 1;
            }
            else if (!strcmp(command, "-l"))
            {
                l_ = 1;
                ex_cmd = 1;
            }
            else if (!strcmp(command, "-la") || !strcmp(command, "-al"))
            {
                a_ = l_ = 1;
                ex_cmd = 1;
            }
            else
            {
                in_cmd = 1;
                char *t_string = init_string(size + 1);
                t_string[size] = '\0';
                char *out_path;
                char *cwd;
                cwd = getcwd(NULL, 0);
                if (command[0] != '~')
                {
                    size_t h_size = strlen(cwd);
                    out_path = init_string(size + 15 + h_size);
                    strcpy(out_path, cwd);
                    strcpy(ff_name, command);
                    strncpy(t_string, command + 1, size - 1);
                    strcat(out_path, t_string);
                    out_path[size + h_size - 1] = '\0';
                    // printf("%s:\n", command);
                    free(t_string);
                    strcpy(out_path, command);
                }
                else
                {
                    size_t h_size = strlen(HOME);
                    out_path = init_string(size + 20 + h_size);
                    strcpy(out_path, HOME);
                    strncpy(t_string, command + 1, size - 1);
                    strcat(out_path, t_string);
                    out_path[size + h_size - 1] = '\0';
                    // printf("%s:\n", command);
                    free(t_string);
                }
                util_ls(out_path);
                free(out_path);
            }
            command = strtok(NULL, "\r\n\t ");
        }
        if (ex_cmd && !in_cmd)
        {
            char *cwd;
            cwd = getcwd(NULL, 0);
            // printf("%s\n", cwd);
            // printf("%s:\n", command);
            util_ls(cwd);
        }
    }
    else
    {
        char *cwd;
        cwd = getcwd(NULL, 0);
        // printf("%s\n", cwd);
        // printf(".:\n");
        util_ls(cwd);
    }
}