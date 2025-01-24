#include "user.h"
#include "kernel/types.h"

int main(int argc, char *argv[]) {
    int pid = fork();

    if (pid > 0) {
        // Parent process
        printf("parent: child=%d\n", pid);
        pid = wait((int *) 0); // Wait for the child process to finish
        printf("child %d is done\n", pid);
    } else if (pid == 0) {
        // Child process
        printf("child: exiting\n");
        exit(0); // Exit child process
    } else {
        // Error in fork
        printf("fork error\n");
    }

    return 0;
}