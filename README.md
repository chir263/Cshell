# C- Shell

> Chirag Jain

> A user-deﬁned interactive shell program that can create and manage new processes.
> The shell is able to create a process out of a system program like emacs, gedit, or any user-deﬁned executable.

## Instructions to compile and run

```
$ make
$ ./main
```

### Speciﬁcation 1

> Shell prompt **<username@system_name:curr_dir>**

### Speciﬁcation 2

> Builtin commands cd, echo, pwd

### Speciﬁcation 3

> **ls** command which lists all the files and directories in the specified directory in alphabetical order
> <br> > **-l** : displays extra information regarding files
> <br> > **-a** : display all files, including hidden files

### Speciﬁcation 4

#### Foreground

> All other commands are treated as system commands like emacs, gedit, and so on. The shell is able to execute them
> either in the background or in the foreground.

> Assumption : time will be displayed for last foreground process only

#### Background

> Any command invoked with "&" is treated as a background command. This implies shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. Whenever a new background process is started, print the PID of the newly created background process on your shell also.

### Speciﬁcation 5

> prints the process info of the given pid.

### Speciﬁcation 6

> If the background process exits then the shell displays the appropriate message to the user.

### Speciﬁcation 7

> custom discover command which emulates the basics of the find command. The command should search for files in a directory hierarchy. The command will have the following optional command line arguments:
> <br> > **-f** : searches for all files
> <br> > **-d** : searches for all directories

```
discover <target_dir> <type_flags> <file_name>
```

### Speciﬁcation 8

> ‘history’ command which is similar to the actual history command

### Speciﬁcation 9

> Using the symbols <, > and >>, the output of commands, usually written to stdout, can be redirected to another file, or the input taken from a file other than stdin. Both input and output redirection can be used simultaneously. Shell support this functionality

### Speciﬁcation 10

> A pipe, identified by |, redirects the output of the command on the left as input to the command on the right. One or more commands can be piped.

### Speciﬁcation 11

> User-defined Commands

### Speciﬁcation 12

> Input/output redirection can occur within command pipelines.

### Speciﬁcation 13

> The tab key can be pressed at any time of your prompt. When you press the tab key, it will print all files which have the same starting letters as the ls command and then again show the same prompt. If there is only one file/folder which has the same starting characters, the prompt should be filled with that. If it's a file, a space should be placed after the file. If it’s a folder, a ‘/’ should be put after the folder name.

### Speciﬁcation 14

> Signal Handling
>
> 1. CTRL-Z It should push any currently running foreground job into the background, and change its state from
>    ‘running’ to ‘stopped.’ This should have no effect on the shell if there is no foreground process running.
> 2. CTRL-C It should interrupt any currently running foreground job, by sending it the SIGINT signal. This should
>    have no effect on the shell if there is no foreground process running.
> 3. CTRL-D It should log you out of your shell, without having any effect on the actual terminal.
