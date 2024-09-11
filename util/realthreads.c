#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* roll_dice() {
	int value = (rand() % 6) + 1;
	int* res_val = malloc(sizeof(int));
	*res_val = value;
	return (void*) res_val;
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
	
	printf("Result: %d %p\n", *res, res);
	free(res);
		
	return 0;
}
