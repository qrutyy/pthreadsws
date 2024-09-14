#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

void* roll_dice() {
	int value = (rand() % 6);
	int* result = malloc(sizeof(int));
	*result = value;
	pthread_exit((void*) result);
}

int main(int argc, char* argv[]) {
	int* res;
	srand(time(NULL));
	pthread_t th;

	if (pthread_create(&th, NULL, &roll_dice, NULL) != 0) {
		return 1;
	}
	if (pthread_join(th, (void**) &res) != 0) {
		return 2;
	}
	printf("Result: %d\n", *res);
	return 0;
}
