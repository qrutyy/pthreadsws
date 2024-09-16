#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void* routine() {
	for (int i = 0; i < 1000000; i ++) {
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th[4];
	int i; 
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 4; i++) {
		if (pthread_create(th + i, NULL, &routine, NULL) != 0) {
			perror("Failed to create a thread\n");
			return 1;
		}
		printf("Thread %d has started\n", i);
	}

	for (i = 0; i < 4; i++) {
		if (pthread_join(th[i], NULL) != 0)
			return 2;
		printf("Thread %d has finished its execution\n", i);
	}		
	// IF YOU CREATE THREADS IN A LOOP - MAKE TWOO SEPARATE LOOPS FOR CREATION AND JOIN
	pthread_mutex_destroy(&mutex);
	printf("Number of mails: %d\n", mails);
	return 0;
}
