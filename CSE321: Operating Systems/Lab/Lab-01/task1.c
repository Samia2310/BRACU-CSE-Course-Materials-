
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int file_descriptor;
    char buffer[256];
    const char *filename;

    if (argc < 2) {
        filename = "TASK-1.c";
    } else {
        filename = argv[1];
    }

    file_descriptor = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);

    if (file_descriptor < 0) {
        printf("Error: Failed to open or create file.\n");
        return 1;
    }

    while (1) {
        printf("Enter string: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error: Failed to read input.\n");
            break;
        }

        if (strcmp(buffer, "-1\n") == 0) {
            break;
        }

        if (write(file_descriptor, buffer, strlen(buffer)) == -1) {
            printf("Error: Failed to write to file.\n");
            break;
        }
    }
    close(file_descriptor);
    printf("'-1' entered. Loop stopped.\n");

    return 0;
}

