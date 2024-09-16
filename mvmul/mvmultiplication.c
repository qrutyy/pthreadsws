#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_MATRIX_VALUE 1000

int matrixdim;
int threadnum;

int **matrix = NULL;
int *ycolumn = NULL;
int *resultcolumn = NULL;

void *matrix_part_computation(void *args) {
	int index = *(int *)args;
	int start_index = (matrixdim / threadnum) * index;
	int res_i = 0;

	for (int i = start_index; i < start_index + (matrixdim / threadnum); i++) {
		for (int j = 0; j < matrixdim; j++) {
			res_i += matrix[i][j] * ycolumn[j];
		}
		resultcolumn[i] = res_i;
		res_i = 0;
	}
	free(args);
	return NULL;
}

void mem_check(void *p) {
	if (p == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(-1);
	}
}

void fill_up_matrix_c(int **matrix, int *column) {
	for (int i = 0; i < matrixdim; i++) {
		column[i] = rand() % 1000;
		for (int j = 0; j < matrixdim; j++) {
			matrix[i][j] = rand() % 1000;
		}
	}
}

void parse_args(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s --n=<matrixdim> --threadnum=<threadnum>\n", argv[0]);
		exit(1);
	}

	if (strncmp(argv[1], "--n=", 4) == 0 && strncmp(argv[2], "--threadnum=", 12) == 0) {
		matrixdim = atoi(argv[1] + 4);
		threadnum = atoi(argv[2] + 12);

		while (matrixdim % threadnum != 0) {
			threadnum -= 1;
		}
		printf("Number of threads after revision: %d\n", threadnum);
	} else {
		fprintf(stderr, "Please use correct arg descriptors\n");
		exit(1);
	}
}

double get_time_in_seconds(void) {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
}

int main(int argc, char *argv[]) {
	parse_args(argc, argv);
	pthread_t *th = malloc(threadnum * sizeof(pthread_t));

	int i = 0;
	srand(time(NULL));

	matrix = malloc(matrixdim * sizeof(int *));
	mem_check((void *)matrix);

	for (i = 0; i < matrixdim; i++) {
		matrix[i] = malloc(matrixdim * sizeof(int));
		mem_check((void *)matrix[i]);
	}

	ycolumn = malloc(matrixdim * sizeof(int));
	mem_check((void *)ycolumn);

	resultcolumn = malloc(matrixdim * sizeof(int));
	mem_check((void *)resultcolumn);

	fill_up_matrix_c(matrix, ycolumn);

	double start_time = get_time_in_seconds();

	for (i = 0; i < threadnum; i++) {
		int *a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &matrix_part_computation, a) != 0) {
			perror("Failed to create a thread\n");
		}
	}

	for (i = 0; i < threadnum; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join a thread\n");
		}
	}

	double end_time = get_time_in_seconds();
	printf("n = %d, threadnum = %d, time = %.6f seconds\n", matrixdim, threadnum, end_time - start_time);

	for (i = 0; i < matrixdim; i++) {
		free(matrix[i]);
	}

	free(matrix);
	free(ycolumn);
	free(resultcolumn);
	free(th);

	return 0;
}
