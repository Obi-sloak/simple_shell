
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROMPT "$ "

void display_prompt(void)
{
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

void run_child_process(char *command)
{
    char *envp[] = {NULL};  
    char *args[2];
    args[0] = command;
    args[1] = NULL;

    if (execve(command, args, envp) == -1)
    {
        perror("Command execution failed");
        exit(EXIT_FAILURE);
    }
}

void execute_command(char *buffer)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        run_child_process(buffer);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}

int main(void)
{
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytesRead;

    while (1)
    {
        display_prompt();

        bytesRead = getline(&buffer, &buffer_size, stdin);

        if (bytesRead <= 0)
        {
            write(STDOUT_FILENO, "\nExiting shell. Goodbye!\n", 26);
            free(buffer);
            exit(EXIT_SUCCESS);
        }

        buffer[bytesRead - 1] = '\0';

        execute_command(buffer);
    }

    free(buffer);
    return 0;
}
