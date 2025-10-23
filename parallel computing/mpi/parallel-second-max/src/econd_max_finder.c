#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define N 100

void find_local_max(int *array, int size, int *local_max) {
    *local_max = -1;
   // *local_second_max = -1;

    if (size > 0) {
        *local_max = array[0];
     //   *local_second_max = -1;

        for (int i = 1; i < size; i++) {
            if (array[i] > *local_max) {
               // *local_second_max = *local_max;
                *local_max = array[i];
       //     } else if (array[i] > *local_second_max) {
         //       *local_second_max = array[i];
           // }
        }
    }
}
}

int main(int argc, char *argv[]) {
    int rank, num_procs;
  
    int *data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int local_size = N / num_procs;
    int *local_data = (int *)malloc(local_size * sizeof(int));
    srand(time(NULL)); 
    if (rank == 0) {
        data = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 100;
            printf("%d ", data[i]);
        }
        printf("\n");
    }

   
    MPI_Scatter(data, local_size, MPI_INT, local_data, local_size, MPI_INT, 0, MPI_COMM_WORLD);

 
    int local_max; //local_second_max;
    find_local_max(local_data, local_size, &local_max);//, &local_second_max);

   // use MPI_MAX Operation
    int global_max;
    MPI_Allreduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    
    for (int i = 0; i < local_size; i++) {
        if (local_data[i] == global_max) {
            local_data[i] = -1;
        }
    }

    find_local_max(local_data, local_size, &local_max);//, &local_second_max);

    //use second time to find the second max
    int global_second_max;
    MPI_Allreduce(&local_max, &global_second_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

 
    if (rank == 0) {
        printf("The second largest element is %d\n", global_second_max);
    }

    free(local_data);
    if (rank == 0) {
        free(data);
    }
    MPI_Finalize();
    return 0;
}
