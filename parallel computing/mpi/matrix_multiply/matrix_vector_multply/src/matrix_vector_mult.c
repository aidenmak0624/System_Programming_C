#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

/*Constants*/

#define MAX_RAND 10
#define MAX_ROW_LENGTH 1024 /*Need to be divisible for P*/
#define MAX_COL_LENGTH 1024


/*Prototypes*/
void init_matrix(int *matrix, int len);
void matrix_mult(int *matrix, int *matrixB, int *result);

/*Functions*/
void init_matrix(int *matrix, int len)
{
    int i;
    printf("\n Matrix generated: ");
printf("\n[");
    for (i = 0; i < len; i++)
    {
        matrix[i] = rand() % MAX_RAND;
        printf(" %d ", matrix[i]);
    }
printf("\n]");
}

void matrix_mult(int *matrix, int *matrixB, int *result)
{
    for (int i = 0; i < MAX_ROW_LENGTH; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < MAX_COL_LENGTH; j++)
        {
            result[i] += matrix[i*MAX_ROW_LENGTH+ j] * matrixB[j];
        }
    }
}

int main(int argc, char *argv[])
{
    int my_rank;
    int num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // grab this process's rank
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // grab the total num of processes

    // create vector
    int *matrix = (int *)malloc(MAX_ROW_LENGTH * MAX_COL_LENGTH * sizeof(int));
    int *matrixB = (int *)malloc(MAX_COL_LENGTH * sizeof(int));
    int *result_matrix = (int *)malloc(MAX_ROW_LENGTH * sizeof(int));
    //int matrix[MAX_ROW_LENGTH][MAX_COL_LENGTH];
    //(int *)malloc(MAX_ROW_LENGTH * MAX_COL_LENGTH * sizeof(int));
    //int matrixB = [MAX_ROW_LENGTH]; // Assume 1 Col-length Matrix
    //(int *)malloc(MAX_COL_LENGTH * sizeof(int));
    //int result_matrix[MAX_ROW_LENGTH]; // Base on assumption, output 1 Col-length Matrix

    srand(time(NULL));
    init_matrix(matrix, MAX_ROW_LENGTH * MAX_COL_LENGTH);
    init_matrix(matrixB, MAX_COL_LENGTH);

    /*Constants*/
    // time variables
    double start_time;
    double stop_time;

    start_time = MPI_Wtime();
    // Calculate Matrix Multiplication
    matrix_mult(matrix, matrixB, result_matrix);
    stop_time = MPI_Wtime();

    double time_used = stop_time - start_time;
    printf("\nCalcuation time: %f\n", time_used);

    /* Free allocated memory */
    free(matrix);
    free(matrixB);
    free(result_matrix);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
