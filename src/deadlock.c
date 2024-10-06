#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 4 // Adjusted to match the number of threads created

pthread_mutex_t mutex_fuel;
pthread_mutex_t mutex_water;
int fuel = 50;

void *routine(void *args) {
  if (rand() % 2 == 0) {
    pthread_mutex_lock(&mutex_fuel);
    sleep(1);
    pthread_mutex_lock(&mutex_water);
  } else {
    pthread_mutex_lock(&mutex_water);
    sleep(1);
    pthread_mutex_lock(&mutex_fuel);
  }
  fuel += 50;

  printf("Increased fuel amount: %d\n", fuel);

  pthread_mutex_unlock(&mutex_fuel);
  pthread_mutex_unlock(&mutex_water);

  return NULL; // Exit the thread
}

int main(int argc, char *argv[]) {
  pthread_t th[THREAD_NUM]; // Create an array of thread identifiers
  int i;

  if (pthread_mutex_init(&mutex_water, NULL) != 0) {
    perror("Mutex initialization failed");
    return 1;
  }

  if (pthread_mutex_init(&mutex_fuel, NULL) != 0) {
    perror("Mutex initialization failed");
    return 1;
  }

  for (i = 0; i < THREAD_NUM; i++) {
    if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
      perror("Failed to create a thread");
      return 1;
    }
    printf("Thread %d has started\n", i);
  }

  for (i = 0; i < THREAD_NUM; i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Failed to join a thread");
      return 2;
    }
    printf("Thread %d has finished its execution\n", i);
  }

  pthread_mutex_destroy(&mutex_fuel);

  pthread_mutex_destroy(&mutex_water);

  printf("Total fuel: %d\n", fuel);

  return 0; // Exit the program
}
