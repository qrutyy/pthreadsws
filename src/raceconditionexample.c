#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
/*An example of race condition between two threads, that suppose to increment mails variable (so it equals 20_000_000)*/
int mails = 0;

void* routine() {
	for (int i = 0; i < 10000000; i ++) {
		mails++;
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t p1, p2;
	if (pthread_create(&p1, NULL, &routine, NULL) != 0)
		return 1;
	if (pthread_create(&p2, NULL, &routine, NULL) != 0)
		return 2;
	if (pthread_join(p1, NULL) != 0) 
		return 3;
	if (pthread_join(p2, NULL) != 0) 
		return 4;
		
	printf("Number of mails: %d\n", mails);
	return 0;
}
