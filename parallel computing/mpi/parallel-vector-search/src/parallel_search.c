#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 256
#define MAX_RAND 10

void search_zero(int * local_vector, int chunk_size, int *local_first_zero, int my_rank){

     for (int i = 0; i < chunk_size; i++) {
        if (local_vector[i] == 0) {
            *local_first_zero = i + my_rank * chunk_size;
            break;
        }
    }
}
int main(int argc, char *argv[])
{
    int my_rank, num_procs, chunk_size;
    int *vector = NULL;
    int local_first_zero = -1, global_first_zero = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

   
    chunk_size = N / num_procs;
    int *local_vector = (int *)malloc(chunk_size * sizeof(int));


    if (my_rank == 0) {
        vector = (int *)malloc(N * sizeof(int));
        printf("Initializing vector:\n");
        for (int i = 0; i < N; i++) {
            vector[i] = rand() % MAX_RAND;
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    
    MPI_Scatter(vector, chunk_size, MPI_DOUBLE, local_vector, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   
    search_zero(local_vector, chunk_size, &local_first_zero, my_rank);
   

    // Use MPI_Reduce - MIN operation
    MPI_Reduce(&local_first_zero, &global_first_zero, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

  
    if (my_rank == 0) {
        if (global_first_zero >= 0) {
            printf("The first occurrence of zero is at index %d.\n", global_first_zero);
        } else {
            printf("No zero found in the vector.\n");
        }
        free(vector);
    }

    free(local_vector);
    MPI_Finalize();
    return 0;
}
