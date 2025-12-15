#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No numbers provided.");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        printf("Error: fork failed.\n");
        return 1;
    }
    else if (pid == 0) {
        if (execvp("./sort", argv) == -1) {
            printf("Error: execvp sort failed.\n");
            exit(1);
        }
    }
    else {
        wait(NULL);

        if (execvp("./oddeven", argv) == -1) {
            printf("Error: execvp oddeven failed.\n");
            return 1;
        }
    }

    return 0;
}
