#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
	int index = *(int*)arg;
	int sum = 0;
	for (int i = 0; i < 5; i ++) {
		sum += primes[index + i];	
	}
	*(int*)arg = sum;
	return (void*)arg;
}

int main(int argc, char* argv[]) {
	pthread_t th[10];
	int i = 0;
	for (i = 0; i < 2; i ++) {
		int* a = malloc(sizeof(int));
		*a = i * 5;
		if (pthread_create(&th[i], NULL, &routine, a) != 0) {
			perror("Failed to create a thread\n");
		}
	}
	int global_sum = 0;
	for (i = 0; i < 2; i ++) {
		void* result;
		if (pthread_join(th[i], &result) != 0) {
			perror("Failed to exit a thread\n");
		}
		global_sum += *(int*)result;
		free(result);
	}
	printf("Global sum: %d", global_sum);
	return 0;
}
