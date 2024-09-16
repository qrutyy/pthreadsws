#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

/*
ISSUE: if we increase the number of cars on the gas station we will see some strange behaviour. Whenever we fuel, at most only 2 cars are waiting for fuel.

EXPLANATION:
pthread_cond_signal - signals one thread to check if it wake up. It doesn't got to the other car. The main issue is caused by a while loop - that is executed only in case fuel variable is locked. That means, that in case its locked - only one thread is checking the statement and after -> unlocks the mutex and waits for it! to change.

If we will increase the ammount of fuel that tank gets we will notice that even if we have enough ammount to fuel up two cars -> it will fuel up only one.

To summarize - mutliple threads are waiting for a signal, but only one gets it.
*/

pthread_mutex_t fuel_mutex;
int fuel_amm; 
pthread_cond_t fuel_cond;
int cars_fueled = 0;

void* fuel_filling(void* arg) {
	for (int i = 0; i < 6 ; i ++) {
		pthread_mutex_lock(&fuel_mutex);
		fuel_amm += 60;
		printf("Filling fuel ... Current ammount: %d\n", fuel_amm);
		pthread_mutex_unlock(&fuel_mutex);
		pthread_cond_broadcast(&fuel_cond);
		sleep(1);
	}
	printf("Current ammount after fuel filling: %d\n", fuel_amm);
	return NULL;
}

void* car(void* arg) {
	pthread_mutex_lock(&fuel_mutex);
	while (fuel_amm < 40) {
		pthread_cond_wait(&fuel_cond, &fuel_mutex);
	}
	printf("Filling the car -40\n");
	cars_fueled++;
	printf("Cars filled: %d\n", cars_fueled);
	fuel_amm -= 40;
	pthread_mutex_unlock(&fuel_mutex);
	printf("Fuel balance: %d\n\n", fuel_amm);
	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_t th[6];
	pthread_mutex_init(&fuel_mutex, NULL);
	pthread_cond_init(&fuel_cond, NULL);
	for (int i = 0; i < 10; i ++ ) {
		if (i == 1 || i == 2) {
			if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
				perror("Failed to create the thread\n");
			}
		} else {
			if (pthread_create(&th[i], NULL, &car, NULL) != 0) {
				perror("Failed to create the thread\n");
			}
		}
	}	
	
	for (int i = 0; i < 5; i ++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join the thread");
		}
	}
	pthread_mutex_destroy(&fuel_mutex);
	pthread_cond_destroy(&fuel_cond);
	return 0;
}

