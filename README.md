# C- Shell

> Chirag Jain (2021101100)

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
