#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 64
#define MAX_RAND 100

void init_matrix_vector(double *matrix, double *vector, int n);
void RowMatrixVectorMultiply(int len, double *a, double *b, double *result, MPI_Comm comm);

void RowMatrixVectorMultiply(int len, double *a, double *b, double *result, MPI_Comm comm)
{
    int i, j;
    int nlocal; /* Number of locally stored rows of A */
    double *fb; /* Will point to a buffer that stores the entire vector b */
    int num_procs, my_rank;
//    MPI_Status status;

    /* Get information about the communicator */
    MPI_Comm_size(comm, &num_procs);
    MPI_Comm_rank(comm, &my_rank);

    /* Allocate the memory that will store the entire vector b */
    fb = (double *)malloc(N * sizeof(double));

    nlocal = N / num_procs;

    /* Gather the entire vector b on each processor using MPI's ALLGATHER operation */
    MPI_Allgather(b, nlocal, MPI_DOUBLE, fb, nlocal, MPI_DOUBLE,
                  comm);

    // Perform the matrix-vector multiplication involving the locally stored submatrix
    for (i = 0; i < nlocal; i++)
    {
        result[i] = 0.0;
        for (j = 0; j < N; j++)
            result[i] += a[i * N + j] * fb[j];
    }

    free(fb);
}

void init_matrix_vector(double *matrix, double *vector, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector[i] = rand() % MAX_RAND;
        for (int j = 0; j < n; j++)
        {
            matrix[i * n + j] = rand() % MAX_RAND;
        }
    }
}

int main(int argc, char *argv[])
{
    int my_rank, num_procs;
    int nlocal;
    double *matrix = NULL, *vector = NULL, *result = NULL;
    double *local_matrix, *local_result, *local_b;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    nlocal = N / num_procs; // Number of rows each process will handle
    local_matrix = (double *)malloc(nlocal * N * sizeof(double));
    local_result = (double *)malloc(nlocal * sizeof(double));
    local_b = (double *)malloc(nlocal * sizeof(double));

 double start_time;
 double end_time;


        if (my_rank == 0)
    {
 
        matrix = (double *)malloc(N * N * sizeof(double)); // Use N*N for easier allocation
        vector = (double *)malloc(N * sizeof(double));
        result = (double *)malloc(N * sizeof(double));
        init_matrix_vector(matrix, vector, N);
	     srand(time(NULL));
    }
 start_time = MPI_Wtime();
    // Scatter rows of the matrix among processes (local*N is the toal number of row* element in row)
    MPI_Scatter(matrix, nlocal * N, MPI_DOUBLE, local_matrix, nlocal * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vector, nlocal, MPI_DOUBLE, local_b, nlocal, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    RowMatrixVectorMultiply(N, local_matrix, local_b, local_result, MPI_COMM_WORLD);


    MPI_Gather(local_result, nlocal, MPI_DOUBLE, result, nlocal, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
 end_time = MPI_Wtime();

        printf("Result vector:\n");
printf("\n[");
        for (int i = 0; i < N; i++)
        {
            printf("%f ", result[i]);
        }
printf("\n]");
 
printf("Execution time :%f",end_time-start_time);
        free(matrix);
        free(result);
        free(vector);
    }

    free(local_b);
    free(local_matrix);
    free(local_result);
    MPI_Finalize();
    return 0;
}

