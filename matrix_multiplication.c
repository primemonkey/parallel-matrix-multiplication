#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 2
#define NUM_THREADS 4

int A[SIZE][SIZE] = {{1, 2}, {3, 4}};
int B[SIZE][SIZE] = {{5, 6}, {7, 8}};
int S[SIZE][SIZE] = {{0, 0}, {0, 0}};
int R[SIZE][SIZE] = {{0, 0}, {0, 0}};

pthread_mutex_t lock;

void *thread_func(void *arg) {
    int i, j, k;
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
            pthread_mutex_lock(&lock);
            if(S[i][j] == 0) {
                S[i][j] = 1;
                pthread_mutex_unlock(&lock);
                for(k = 0; k < SIZE; k++) {
                    R[i][j] += A[i][k] * B[k][j];
                }
                S[i][j] = 2;
            } else {
                pthread_mutex_unlock(&lock);
            }
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    pthread_mutex_init(&lock, NULL);

    for(i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }

    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    for(i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%d ", R[i][j]);
        }
        printf("\n");
    }

    return 0;
}
