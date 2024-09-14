#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MATRIX_VALUE 1000

int matrixdim;
int threadnum;

int **matrix = NULL;
int *ycolumn = NULL;
int *resultcolumn = NULL;

void* matrix_part_computation(void* args) {
	int index = *(int*)args;
	int start_index = (matrixdim / threadnum) * index;
	int res_i = 0;
	for (int i = start_index; i < start_index + (matrixdim / threadnum); i++) {
		for (int j = 0; j < matrixdim; j++) {
			res_i += matrix[i][j] * ycolumn[j]; 
		}
		ycolumn[i] = res_i;
		res_i = 0;
	}
	free(args);
}

void mem_check(void* p) {
	if (p == NULL) {
		fprintf(stderr, "Memmory allocation failed\n");
		exit(-1);
	}

}

void fill_up_matrix_c(int** matrix, int* column) {
	for (int i = 0; i < matrixdim; i++) {
		column[i] = rand() % 1000;
		for (int j = 0; j < matrixdim; j++) {
			matrix[i][j] = rand() % 1000;
		}
	}
}

void parse_args(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Wrong ammount of args passed in\n");
		exit(1);
	}

	if (strcmp(argv[1], "--n") + strcmp(argv[2], "--threadnum") < 2) {
		fprintf(stderr, "Please use arg descriptors\n");
		exit(1);
	} else {
		matrixdim = atoi(argv[1] + 4);
		threadnum = atoi(argv[2] + 12);
		if (matrixdim % threadnum != 0) {
			fprintf(stderr, "Matrixdim should be diveded by number of threads)\n Mb will be edited in future\n");
			exit(0);
		}
	}
}

void print_comp_data() {
	for (int i = 0; i < matrixdim; i ++) {
		for (int j = 0; j < matrixdim; j ++) {
			printf("%d ", matrix[i][j]);
		}
		printf("# %d # %d", ycolumn[i], resultcolumn[i]);
	        printf("\n");
	}
}

int main(int argc, char* argv[]) {
	parse_args(argc, argv);
	pthread_t th[threadnum];
	int i = 0;
	srand(time(NULL));

	matrix = malloc(matrixdim * sizeof(int));
	mem_check((void*)matrix);

	for (i = 0; i < matrixdim; i++) {
		matrix[i] = malloc(matrixdim * sizeof(int));
		mem_check((void*)matrix);
	}

	ycolumn = malloc(matrixdim * sizeof(int));
	mem_check((void*)matrix);

	resultcolumn = malloc(matrixdim * sizeof(int));
	mem_check((void*)matrix);

	fill_up_matrix_c(matrix, ycolumn);

	for (i = 0; i < threadnum; i++) {
		int* a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &matrix_part_computation, a) != 0) {
			perror("Failed to ccreate a thread\n");
		}
	}

	for (i = 0; i < threadnum; i ++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join a thread\n");
		}
	}

	print_comp_data(resultcolumn);

// 	free_matrix_and_comp(matrix, ycolumn, resultcolumn);

	return 0;
}

