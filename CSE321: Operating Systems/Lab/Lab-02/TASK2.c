#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ARGS 5

void* generate_fibonacci(void* arg);
void* search_fibonacci(void* arg);

int main() {
    int n, s;
    pthread_t fib_thread, search_thread;

    printf("Enter the term of fibonacci sequence:\n");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return 1;
    }

    if (n < 0 || n > 40) {
        fprintf(stderr, "Error: n must be between 0 and 40.\n");
        return 1;
    }

    void** fib_args = (void**)malloc(MAX_ARGS * sizeof(void*));
    if (fib_args == NULL) {
        perror("Failed to allocate memory for thread arguments");
        return 1;
    }
    int* n_ptr = (int*)malloc(sizeof(int));
    if (n_ptr == NULL) {
        perror("Failed to allocate memory for n");
        free(fib_args);
        return 1;
    }
    *n_ptr = n;
    fib_args[0] = (void*)n_ptr;

    if (pthread_create(&fib_thread, NULL, generate_fibonacci, (void*)fib_args) != 0) {
        perror("Failed to create Fibonacci thread");
        free(n_ptr);
        free(fib_args);
        return 1;
    }

    int* fib_sequence;
    if (pthread_join(fib_thread, (void**)&fib_sequence) != 0) {
        perror("Failed to join Fibonacci thread");
        return 1;
    }

    printf("\n");

    printf("How many numbers you are willing to search?:\n");
    if (scanf("%d", &s) != 1) {
        fprintf(stderr, "Invalid input.\n");
        free(fib_sequence);
        return 1;
    }

    if (s <= 0) {
        fprintf(stderr, "Error: The number of searches must be greater than 0.\n");
        free(fib_sequence);
        return 1;
    }

    int* search_indices = (int*)malloc(s * sizeof(int));
    if (search_indices == NULL) {
        perror("Failed to allocate memory for search indices");
        free(fib_sequence);
        return 1;
    }

    for (int i = 0; i < s; i++) {
        printf("Enter search %d:\n", i + 1);
        if (scanf("%d", &search_indices[i]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            free(fib_sequence);
            free(search_indices);
            return 1;
        }
    }

    void** search_args = (void**)malloc(MAX_ARGS * sizeof(void*));
    if (search_args == NULL) {
        perror("Failed to allocate memory for thread arguments");
        free(fib_sequence);
        free(search_indices);
        return 1;
    }
    int* s_ptr = (int*)malloc(sizeof(int));
    if (s_ptr == NULL) {
        perror("Failed to allocate memory for s");
        free(fib_sequence);
        free(search_indices);
        free(search_args);
        return 1;
    }
    *s_ptr = s;
    int* n_ptr_search = (int*)malloc(sizeof(int));
    if (n_ptr_search == NULL) {
        perror("Failed to allocate memory for n_ptr_search");
        free(fib_sequence);
        free(search_indices);
        free(search_args);
        free(s_ptr);
        return 1;
    }
    *n_ptr_search = n + 1;

    search_args[0] = (void*)fib_sequence;
    search_args[1] = (void*)search_indices;
    search_args[2] = (void*)s_ptr;
    search_args[3] = (void*)n_ptr_search;

    if (pthread_create(&search_thread, NULL, search_fibonacci, (void*)search_args) != 0) {
        perror("Failed to create search thread");
        free(fib_sequence);
        free(search_indices);
        free(s_ptr);
        free(n_ptr_search);
        free(search_args);
        return 1;
    }

    int* search_results;
    if (pthread_join(search_thread, (void**)&search_results) != 0) {
        perror("Failed to join search thread");
        free(fib_sequence);
        free(search_indices);
        return 1;
    }

    printf("\nSearch Results:\n");
    for (int i = 0; i < s; i++) {
        printf("result of search #%d = %d\n", i + 1, search_results[i]);
    }

    free(fib_sequence);
    free(search_indices);
    free(search_results);

    return 0;
}

void* generate_fibonacci(void* arg) {
    void** args = (void**)arg;
    int n = *(((int*)args[0]));

    int* fib_sequence = (int*)malloc((n + 1) * sizeof(int));
    if (fib_sequence == NULL) {
        perror("Failed to allocate memory for Fibonacci sequence");
        pthread_exit(NULL);
    }

    if (n >= 0) {
        fib_sequence[0] = 0;
        printf("a[0] = %d\n", fib_sequence[0]);
    }
    if (n >= 1) {
        fib_sequence[1] = 1;
        printf("a[1] = %d\n", fib_sequence[1]);
    }

    for (int i = 2; i <= n; i++) {
        fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
        printf("a[%d] = %d\n", i, fib_sequence[i]);
    }

    free(args[0]);
    free(args);

    return (void*)fib_sequence;
}

void* search_fibonacci(void* arg) {
    void** args = (void**)arg;
    int* fib_sequence = (int*)args[0];
    int* search_indices = (int*)args[1];
    int num_searches = *((int*)args[2]);
    int sequence_length = *((int*)args[3]);

    int* search_results = (int*)malloc(num_searches * sizeof(int));
    if (search_results == NULL) {
        perror("Failed to allocate memory for search results");
        pthread_exit(NULL);
    }

    for (int i = 0; i < num_searches; i++) {
        int index = search_indices[i];
        if (index >= 0 && index < sequence_length) {
            search_results[i] = fib_sequence[index];
        } else {
            search_results[i] = -1;
        }
    }

    free(args[2]);
    free(args[3]);
    free(args);

    return (void*)search_results;
}
