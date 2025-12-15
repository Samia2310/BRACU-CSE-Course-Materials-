#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t child_pid;
    int grandchild_counter;

    printf("1. Parent process ID : %d\n", getpid());

    child_pid = fork();

    if (child_pid < 0) {
        printf("Error: Parent fork failed.\n");
        return 1;
    }

    if (child_pid == 0) {
        printf("2. Child process ID: %d\n", getpid());

        for (grandchild_counter = 0; grandchild_counter < 3; grandchild_counter++) {
            pid_t grandchild_pid = fork();

            if (grandchild_pid < 0) {
                printf("Error: Grandchild fork failed.\n");
                exit(1);
            }

            if (grandchild_pid == 0) {
                printf("%d. Grand Child process ID: %d\n", grandchild_counter + 3, getpid());
                exit(0);
            }
        }

        for (grandchild_counter = 0; grandchild_counter < 3; grandchild_counter++) {
            wait(NULL);
        }
    } else {
        wait(NULL);
    }

    return 0;
}
