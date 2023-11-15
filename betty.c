#include <unistd.h>

/**
 * main - Entry point of the program
 *
 * Return: 0 on success, non-zero on failure
 */
int main(void)
{
    char *message = "Hello, Betty!\n";
    int len = 13;

    /* Write the message to the standard output */
    write(1, message, len);

    return (0);
}
