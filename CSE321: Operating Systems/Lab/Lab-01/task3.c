#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FILENAME "processCounts.txt"

void write_to_file(pid_t pid) {
    int fd = open(FILENAME, O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file for writing");
        return;
    }

    char buffer[100];
    int len = sprintf(buffer, "PID: %d\n", pid);

    if (write(fd, buffer, len) == -1) {
        perror("Error writing to file");
    }
    close(fd);
}

int main() {
    int fd = open(FILENAME, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error creating file");
        exit(1);
    }
    close(fd);

    pid_t a, b, c;
    pid_t d = -1, e = -1, f = -1;

    write_to_file(getpid());

    a = fork();
    b = fork();
    c = fork();

    if (a == 0) {
        if (getpid() % 2 != 0) {
            d = fork();
            if (d == 0) {
                write_to_file(getpid());
                exit(0);
            } else if (d > 0) {
                wait(NULL);
            }
        }
        write_to_file(getpid());
        exit(0);
    }


    if (b == 0) {
        if (getpid() % 2 != 0) {
            e = fork();
            if (e == 0) {
                write_to_file(getpid());
                exit(0);
            } else if (e > 0) {
                wait(NULL);
            }
        }
        write_to_file(getpid());
        exit(0);
    }

    if (c == 0) {
        if (getpid() % 2 != 0) {
            f = fork();
            if (f == 0) {
                write_to_file(getpid());
                exit(0);
            } else if (f > 0) {
                wait(NULL);
            }
        }
        write_to_file(getpid());
        exit(0);
    }

    if (a > 0 && b > 0 && c > 0) {
        while (wait(NULL) > 0);

        printf("\nAll processes finished. Reading results from %s\n", FILENAME);

        int fileDescriptor = open(FILENAME, O_RDONLY);
        if (fileDescriptor == -1) {
            perror("Error opening file for reading");
            exit(1);
        }

        char bufferSize[4096];
        ssize_t fileContent;
        int total_count = 0;

        while ((fileContent = read(fileDescriptor, bufferSize, sizeof(bufferSize) - 1)) > 0) {
            bufferSize[fileContent] = '\0';
            printf("%s", bufferSize);

            for (int i = 0; i < fileContent; i++) {
                if (bufferSize[i] == '\n') {
                    total_count++;
                }
            }
        }
        if (fileContent == -1) {
            perror("Error reading file");
        }

        close(fileDescriptor);
        printf("Total processes created: %d\n", total_count);
        unlink(FILENAME);
    }

    return 0;
}