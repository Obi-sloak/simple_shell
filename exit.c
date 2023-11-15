#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct {
    char **args;
    int status;
} data_shell;

unsigned int _atoi(const char *str);
int _isdigit(const char *str);
size_t _strlen(const char *str);
void get_error(data_shell *datash, int error_code);

int exit_shell(data_shell *datash) {
    if (datash->args[1] != NULL) {
        unsigned int ustatus = _atoi(datash->args[1]);
        int is_digit = _isdigit(datash->args[1]);
        size_t str_len = _strlen(datash->args[1]);
        int big_number = ustatus > (unsigned int)INT_MAX;

        if (!is_digit || str_len > 10 || big_number) {
            get_error(datash, 2);
            datash->status = 2;
            return 1;
        }

        datash->status = ustatus % 256;
    }

    return 0;
}







