#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *routine() {
	printf("Test from threads\n");
	sleep(3);
	printf("Ending thread\n");
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	return 0;
}
