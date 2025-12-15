#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 

void check_odd_even(int arr[], int n) {
    printf("Odd or Even:\n");
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            printf("%d is Even\n", arr[i]);
        } else {
            printf("%d is Odd\n", arr[i]);
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
   
    check_odd_even(numbers, num_count);

    free(numbers); 
    return 0;
}
