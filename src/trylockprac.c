#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

/*
This program simulates a scenario where multiple "chefs" (represented by threads) are competing to use a limited number of stoves. Each stove has a finite amount of fuel, and the chefs try to use the stoves in parallel, consuming fuel in the process. The program uses mutexes to ensure that only one chef can access a stove at a time, preventing race conditions.
*/

pthread_mutex_t stove_mutex[4];
int stove_fuel[4] = { 100, 100, 100, 100 };

void* routine(void* arg) {
	for (int i = 0; i < 4; i ++) {
		if (pthread_mutex_trylock(&stove_mutex[i]) == 0) {

			int fuel_needed = (rand() % 50);

			if (stove_fuel[i] - fuel_needed < 0) {
				printf("No more fuel\n");
			} else {
				stove_fuel[i] -= fuel_needed;
				usleep(500000);
				printf("Fuel left: %d Used stove nr. %d\n", stove_fuel[i], i + 1);
			}

			pthread_mutex_unlock(&stove_mutex[i]);
			break;
		} else {
			if (i == 3) {
				printf("No stove available yet ...\n");
				usleep(300000);
				i = -1;
			}
		}
	}
}


int main(int argc, char* argv[]) {
	srand(time(NULL));
	pthread_t th[30];
	for (int i = 0; i < 4; i ++) {
		pthread_mutex_init(&stove_mutex[i], NULL);
	}

	for (int i = 0; i < 30; i ++) {
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
			perror("Failed to create a thread\n");
		}
	}
	
	for (int i = 0; i < 30; i ++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join a thread\n");
		}
	}

	for (int i = 0; i < 4; i ++) {
		printf("Balance at stove nr %d = %d\n", i + 1, stove_fuel[i]);
		pthread_mutex_destroy(&stove_mutex[i]);
	}
	return 0;
}

