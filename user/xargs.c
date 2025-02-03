#include "kernel/types.h"
#include "user/user.h"

#define MAXARG 32  // Maximum number of arguments
#define MAXLEN 512 // Maximum length of an input line

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [arg...]\n");
        exit(1);
    }

    char *new_argv[MAXARG];
    for (int i = 1; i < argc; i++) {
        new_argv[i - 1] = argv[i]; // Copy the command and initial arguments
    }

    char buf[MAXLEN];
    int len = 0;

    while (read(0, buf + len, 1) == 1) {
        if (buf[len] == '\n') { // When encountering a newline, execute the command
            buf[len] = 0; // Null-terminate the string
            new_argv[argc - 1] = buf; // Add the line to the last argument
            new_argv[argc] = 0; // Null-terminated argument list

            // Check if new_argv[0] is valid before execution
            if (new_argv[0] == 0) {
                fprintf(2, "xargs: no command provided\n");
                exit(1);
            }

            if (fork() == 0) { // Create a child process to execute the command
                exec(new_argv[0], new_argv);
                fprintf(2, "exec %s failed\n", new_argv[0]);
                exit(1);
            } else {
                wait(0); // Wait for the child process to finish
            }
            len = 0; // Reset buffer
        } else {
            len++;
            if (len >= MAXLEN) { // Limit input length
                fprintf(2, "xargs: input too long\n");
                exit(1);
            }
        }
    }
    exit(0);
}
