#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define THREAD_NUM 8

int dice_values[8];
int status[8] = { 0 };

pthread_barrier_t barrier_rolled_dice;
pthread_barrier_t barrier_calculated;

void* rollDice(void* args) {
	int index = *(int*)args;
	dice_values[index] = rand() % 6 + 1;
	pthread_barrier_wait(&barrier_rolled_dice);
	pthread_barrier_wait(&barrier_calculated);
	if (status[index] == 1) {
		printf("(thread nr. %d rolled %d) I won\n", index, dice_values[index]);
	} else {
		printf("(thread nr. %d rolled %d) I lost\n", index, dice_values[index]);
	}
	free(args);
	return NULL;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	pthread_t th[THREAD_NUM];
	int i;
	pthread_barrier_init(&barrier_rolled_dice, NULL, THREAD_NUM + 1); // bc we need to wait for the main thread also 
	pthread_barrier_init(&barrier_calculated, NULL, THREAD_NUM + 1);
	for (i = 0; i < THREAD_NUM; i++) {
		int* a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &rollDice, a) != 0) {
			perror("Failed to create a thread\n");
		}
	}
	pthread_barrier_wait(&barrier_rolled_dice);
	
	int max = 0;
	for (i = 0; i < THREAD_NUM; i ++) {
		if (dice_values[i] > max) max = dice_values[i];
	}

	for (i = 0; i < THREAD_NUM; i ++) {
		status[i] = dice_values[i] == max ? 1 : 0;	
	}

	pthread_barrier_wait(&barrier_calculated);

	for (i = 0; i < THREAD_NUM; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join a thread\n");
		}
	}
	pthread_barrier_destroy(&barrier_rolled_dice);
	pthread_barrier_destroy(&barrier_calculated);
	return 0;
}
