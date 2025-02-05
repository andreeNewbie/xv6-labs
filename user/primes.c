#include "user.h"

void primes(int p[2]) __attribute__((noreturn)); 

void primes(int p[2]) {
    close(p[1]); // Close write end of the pipe in this process

    int prime;
    if (read(p[0], &prime, sizeof(int)) <= 0) {
        close(p[0]);
        exit(0); // Exit if no more data
    }

    printf("prime %d\n", prime);

    int next_pipe[2];
    if (pipe(next_pipe) == -1) {
        fprintf(2, "Failed to create pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Failed to fork\n");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(p[0]); // Close the read end of the current pipe
        primes(next_pipe);
    } else {
        // Parent process
        close(next_pipe[0]); // Close read end of the next pipe

        int num;
        while (read(p[0], &num, sizeof(int)) > 0) {
            if (num % prime != 0) {
                write(next_pipe[1], &num, sizeof(int));
            }
        }

        close(p[0]);
        close(next_pipe[1]); // Close write end of the next pipe
        wait(0); // Wait for the child process to finish
        exit(0);
    }
}

int main() {
    int p[2];
    if (pipe(p) == -1) {
        fprintf(2, "Failed to create pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Failed to fork\n");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        primes(p);
    } else {
        // Parent process
        close(p[0]); // Close read end of the pipe

        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }

        close(p[1]); // Close write end of the pipe
        wait(0); // Wait for the child process to finish
        exit(0);
    }
}
