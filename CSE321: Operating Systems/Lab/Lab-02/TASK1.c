#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define totalStudent 10
#define totalChairs 3

pthread_mutex_t mutex;
sem_t studentAvailable;
sem_t ST_available;
sem_t allow_next_arrival;

int waiting_students = 0;
int served_students = 0;

void* ST_func(void* arg);
void* student_func(void* id);

int main() {
    pthread_t ST_thread;
    pthread_t student_threads[totalStudent];
    int ids[totalStudent];
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    sem_init(&studentAvailable, 0, 0);
    sem_init(&ST_available, 0, 0);
    sem_init(&allow_next_arrival, 0, 0);
    pthread_create(&ST_thread, NULL, ST_func, NULL);
    ids[0] = 0;
    pthread_create(&student_threads[0], NULL, student_func, &ids[0]);

    for (int i = 1; i < totalStudent; i++) {
        sem_wait(&allow_next_arrival);
        ids[i] = i;
        pthread_create(&student_threads[i], NULL, student_func, &ids[i]);
    }

    for (int i = 0; i < totalStudent; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_join(ST_thread, NULL);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&studentAvailable);
    sem_destroy(&ST_available);
    sem_destroy(&allow_next_arrival);

    return 0;
}

void* ST_func(void* arg) {
    while (served_students < totalStudent) {
        sem_wait(&studentAvailable);

        pthread_mutex_lock(&mutex);
        waiting_students--;
        printf("A waiting student started getting consultation\n");
        printf("Number of students now waiting: %d\n", waiting_students);
        pthread_mutex_unlock(&mutex);

        sem_post(&ST_available);
        printf("ST giving consultation\n");
        sleep(1);
    }
    pthread_exit(NULL);
}

void* student_func(void* id) {
    int student_id = *(int*)id;

    sleep(1);

    pthread_mutex_lock(&mutex);
    if (waiting_students < totalChairs) {
        waiting_students++;
        printf("Student %d started waiting for consultation\n", student_id);

        if (student_id < totalStudent - 1) {
            sem_post(&allow_next_arrival);
        }

        pthread_mutex_unlock(&mutex);

        sem_post(&studentAvailable);
        sem_wait(&ST_available);
        printf("Student %d is getting consultation\n", student_id);
        sleep(1);

        pthread_mutex_lock(&mutex);
        served_students++;
        printf("Student %d finished getting consultation and left\n", student_id);
        printf("Number of served students: %d\n", served_students);

        if (student_id + 2 < totalStudent) {
            sem_post(&allow_next_arrival);
        }
        pthread_mutex_unlock(&mutex);

    } else {
        printf("No chairs remaining in lobby. Student %d Leaving.....\n", student_id);
        printf("Student %d finished getting consultation and left\n", student_id);
        served_students++;
        printf("Number of served students: %d\n", served_students);

        pthread_mutex_unlock(&mutex);

        if (student_id < totalStudent - 1) {
            sem_post(&allow_next_arrival);
        }
    }
    pthread_exit(NULL);
}
