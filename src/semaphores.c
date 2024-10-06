#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 5

sem_t semaphore;

void* routine(void* args) {
  sem_wait(&semaphore);
  sleep(1);
  printf("HELLO FROM THREAD %d\n", *(int*)args);
  sem_post(&semaphore);
  free(args);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t th[THREAD_NUM];
  sem_init(&semaphore, 0, 2);
  int i = 0;

  for (i = 0; i < THREAD_NUM; i ++) {
    int *a = malloc(sizeof(int));
    *a = i;
    if (pthread_create(&th[i], NULL, &routine, a) != 0) {
      perror("Failed to create a thread\n");
    }
  }
  
  for (i = 0; i < THREAD_NUM; i ++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Failed to pthread_join a thread\n");
    }
  }

  sem_destroy(&semaphore);
  return 0;
}
