#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void initialize_matrices(int **A, int **B, int **C, int n) {
    #pragma omp for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = 1;
            B[i][j] = 2;
            C[i][j] = 0;
        }
    }
}

void matrix_multiply(int **A, int **B, int **C, int n) {
    #pragma omp for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 100; // Matrix size
    int num_threads[] = {2, 4, 8, 16}; // Thread counts for experimentation

    // Allocate memory for matrices
    int **A = (int **)malloc(n * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    int **C = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        A[i] = (int *)malloc(n * sizeof(int));
        B[i] = (int *)malloc(n * sizeof(int));
        C[i] = (int *)malloc(n * sizeof(int));
    }

    printf("Threads\tExecution Time (seconds)\n");
    for (int t = 0; t < 4; t++) {
        omp_set_num_threads(num_threads[t]);

        double start = omp_get_wtime();

        // Parallel region
        #pragma omp parallel
        {
            initialize_matrices(A, B, C, n);
            matrix_multiply(A, B, C, n);
        }

        double end = omp_get_wtime();

        printf("%d\t%.5f\n", num_threads[t], end - start);
    }

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}

