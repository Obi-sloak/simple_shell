#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) {
    char *buffer = NULL;
    size_t buffer_size = 0;

    while (1) {
        
        write(STDOUT_FILENO, "$ ", 2);

       
        ssize_t read_bytes = getline(&buffer, &buffer_size, stdin);

        
        if (read_bytes == -1) {
            if (feof(stdin)) {
                write(STDOUT_FILENO, "\n", 1);
                free(buffer);
                exit(EXIT_SUCCESS);
            }
            perror("getline");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        
        if (buffer[read_bytes - 1] == '\n') {
            buffer[read_bytes - 1] = '\0';
        }

        
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            free(buffer);
            exit(EXIT_FAILURE);
        } else if (pid == 0) { 
            
            char *args[2];
            args[0] = buffer;
            args[1] = NULL;

          
            char *path = getenv("PATH");
            char *path_copy = strdup(path);
            char *dir = strtok(path_copy, ":");

            while (dir != NULL) {
                char executable_path[BUFFER_SIZE];
                strcpy(executable_path, dir);
                strcat(executable_path, "/");
                strcat(executable_path, args[0]);

            
                if (access(executable_path, X_OK) == 0) {
                    execv(executable_path, args);
                    perror("execv");
                    free(path_copy);
                    free(buffer);
                    exit(EXIT_FAILURE);
                }

                dir = strtok(NULL, ":");
            }

            
            write(STDERR_FILENO, "Command not found: ", 19);
            write(STDERR_FILENO, args[0], strlen(args[0]));
            write(STDERR_FILENO, "\n", 1);
            free(path_copy);
            free(buffer);
            exit(EXIT_FAILURE);
        } else { 
            
            wait(NULL);
        }
    }

    
    return 0;
}
