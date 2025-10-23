#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//#include <limits.h>

void floyd(int **A, int n)
{
    #pragma omp parallel for schedule(static)
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (A[i][j] > A[i][k] + A[k][j])
                {
                    A[i][j] = A[i][k] + A[k][j];
                }
            }
        }
    }
}

void init_A(int **A, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                A[i][j] = 0;
            }
            else
            {
                A[i][j] = (rand() % 10) + 1; // Random weights for edges
            }
        }
    }
}

int main()
{
    int n_iter[] = {10};  // Sizes of matrices to test
    int num_threads[] = {4, 8, 16}; // Number of threads to test

    for (int q = 0; q < 3; q++)  // Loop over different matrix sizes
    {
        int n = n_iter[q];
        int **A = (int **)malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            A[i] = (int *)malloc(n * sizeof(int));
        }

        init_A(A, n); // Initialize the matrix

        for (int t = 0; t < 3; t++)  // Loop over number of threads
        {
            omp_set_num_threads(num_threads[t]); // Set number of threads
            
            double start = omp_get_wtime(); // Start timing
	    #pragma omp parallel
	    {
            floyd(A, n);  // Perform Floyd's algorithm
  	    }
            double end = omp_get_wtime(); // End timing
 for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    printf("A[%d][%d]: %d\n", i, j, A[i][j]); 
                }
            }

            printf("Matrix size: %d x %d, Threads: %d, Execution time: %.5f seconds\n", n, n, num_threads[t], end - start);
        }

        // Free the matrix memory
        for (int i = 0; i < n; i++)
        {
            free(A[i]);
        }
        free(A);
    }

    return 0;
}

