#include <stdio.h>
#include <stdlib.h> 

void sort_descending(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j+1]) { 
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1; 
    }

    int num_count = argc - 1;
    int *numbers = (int *)malloc(num_count * sizeof(int));

    if (numbers == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    for (int i = 0; i < num_count; i++) {
        numbers[i] = atoi(argv[i+1]); 
    }

    sort_descending(numbers, num_count);

    printf("The sorted array in descending order is:\n");
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    free(numbers);
    return 0;
}
