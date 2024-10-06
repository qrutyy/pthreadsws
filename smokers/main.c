#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS_NUM 3

sem_t table_ready;

pthread_mutex_t tobacco_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match_m = PTHREAD_MUTEX_INITIALIZER;

void* tobacco_routine(void *args) {
  while (1) {
    pthread_mutex_lock(&tobacco_m);
    printf("Smoker with tobacco makes a cig and smokes\n");
    sem_post(&table_ready);
    sleep(2);
  } 
  return NULL;
}

void* paper_routine(void *args) {
  while (1) {
    pthread_mutex_lock(&paper_m);
    printf("Smoker with paper makes a cig and smokes\n");
    sem_post(&table_ready);
    sleep(2);
  } 
  return NULL;
}

void* match_routine(void *args) {
  printf("Entered match_routine\n");
  while (1) {
    pthread_mutex_lock(&match_m); // it tries to lock, but its already locked, so it waits for unlock from agent_routine
    printf("Smoker with matches makes a cig and smokes\n");
    sem_post(&table_ready);
    sleep(2);
  }  
  return NULL;
}

void* agent_routine(void *args) {
  while (1) {
    sem_wait(&table_ready);
    int item = rand() % 3;

    switch (item) {
      case 0:
        printf("Agent puts tobacco and paper\n");
        pthread_mutex_unlock(&match_m);
        break;
      case 1:
        printf("Agent puts tobacco and matches\n");
        pthread_mutex_unlock(&paper_m);
        break;
      case 2:
        printf("Agent puts matches and paper\n");
        pthread_mutex_unlock(&tobacco_m);
        break;
    }
    sleep(2);
  }
  return NULL;
}


int main(int argc, char *argv[]) {
  pthread_t tobacco_man_th;
  pthread_t paper_man_th;
  pthread_t match_man_th;
  pthread_t agent_th;

  sem_init(&table_ready, 0, 3);

  pthread_mutex_lock(&tobacco_m);
  pthread_mutex_lock(&match_m);
  pthread_mutex_lock(&paper_m);

  if (pthread_create(&agent_th, NULL, &agent_routine, NULL) != 0) {
    perror("Failed to create a thread\n");
  }

  if (pthread_create(&tobacco_man_th, NULL, &tobacco_routine, NULL) != 0) {
    perror("Failed to create a thread\n");
  }
  if (pthread_create(&paper_man_th, NULL, &paper_routine, NULL) != 0) {
    perror("Failed to create a thread\n");
  }
  if (pthread_create(&match_man_th, NULL, &match_routine, NULL) != 0) {
    perror("Failed to create a thread\n");
  }

 if (pthread_join(agent_th, NULL) != 0) {
    perror("Failed to join a thread\n");
  }

  if (pthread_join(tobacco_man_th, NULL) != 0) {
    perror("Failed to join a thread\n");
  }
  if (pthread_join(paper_man_th, NULL) != 0) {
    perror("Failed to join a thread\n");
  }
  if (pthread_join(match_man_th, NULL) != 0) {
    perror("Failed to join a thread\n");
  }
 

  sem_destroy(&table_ready);
  return 0;
}
