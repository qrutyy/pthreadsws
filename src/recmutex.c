#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUM 4 // Adjusted to match the number of threads created

pthread_mutex_t mutex_fuel;
int fuel = 50;

void *routine(void *args) {
  pthread_mutex_lock(&mutex_fuel);
  pthread_mutex_lock(&mutex_fuel); // try to lock multiple times
  fuel+=50;
  printf("Increased fuel to: %d\n", fuel);
  pthread_mutex_unlock(&mutex_fuel);
  pthread_mutex_unlock(&mutex_fuel); // reccursive mutexes solve this issue, but u need to unlock the same ammount of time as lock.
  return NULL; // Exit the thread
}

int main(int argc, char *argv[]) {
  pthread_t th[THREAD_NUM]; // Create an array of thread identifiers
  int i;
  pthread_mutexattr_t recursiveMutexAtributes;
  pthread_mutexattr_init(&recursiveMutexAtributes);
  pthread_mutexattr_settype(&recursiveMutexAtributes, PTHREAD_MUTEX_RECURSIVE);


  if (pthread_mutex_init(&mutex_fuel, &recursiveMutexAtributes) != 0) {
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
  pthread_mutexattr_destroy(&recursiveMutexAtributes);

  printf("Total fuel: %d\n", fuel);

  return 0; // Exit the program
}
