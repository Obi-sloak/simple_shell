#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 64

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0
 */
int main(void)
{
    char *buffer = NULL;
    ssize_t read_bytes;
    size_t buffer_size = 0;

    while ((read_bytes = getline(&buffer, &buffer_size, stdin)) != -1)
    {
        
        if (buffer[read_bytes - 1] == '\n')
            buffer[read_bytes - 1] = '\0';

        
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) 
        {
            
            char *args[MAX_ARGS];
            char *token = strtok(buffer, " ");
            int i = 0;

            while (token != NULL && i < MAX_ARGS - 1)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }

            args[i] = NULL;

            
            char *path = getenv("PATH");
            char *path_copy = strdup(path);
            char *dir = strtok(path_copy, ":");

            while (dir != NULL)
            {
                char executable_path[BUFFER_SIZE];
                
                char *exe_path_ptr = executable_path;
                while ((*exe_path_ptr++ = *dir++))
                    ;
                exe_path_ptr[-1] = '/';
                char *cmd_ptr = args[0];
                while ((*exe_path_ptr++ = *cmd_ptr++))
                    ;

                
                if (access(executable_path, X_OK) == 0)
                {
                    execve(executable_path, args, NULL);
                    perror("execve");
                    exit(EXIT_FAILURE);
                }

                dir = strtok(NULL, ":");
            }

            
            write(STDERR_FILENO, "Command not found: ", 19);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, "\n", 1);
            free(path_copy);
            exit(EXIT_FAILURE);
        }
        else 
        {
           
            wait(NULL);

            
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "$ ", 2);
        }
    }

    free(buffer);  
    return 0;
}

