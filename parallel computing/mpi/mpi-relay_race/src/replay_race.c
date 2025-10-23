// open mpi environment
// if (my_rank ==0)
//{
// create vector
// start Team 1 time
//  Send T1 VEC - 1
// start Team 2 time
//  SEnd T2 VEC - 2
//  MPI_ RECEIVE ( ANY SOURCE, ANY TAG) X2
// if(TAG ==1)
//  TIME ENF 1
// ELSE(TAG ==2)
//  TIME END 2
//  compare time for both team
//  print who win

// ELSE IF TEAM == odd
// recv vec
// for(n){
// if((myrank+2)<=p-1
// send to my_rank+1
// else
//  send to 0
// }

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

/*Constants*/
#define N 10
#define MAX_RAND 100

/*Prototypes*/
void init_vec(int *vec, int len);

/*Functions*/
void init_vec(int *vec, int len)
{
    int i;
    printf("\nvec: ");
    for (i = 0; i < len; i++)
    {
        vec[i] = rand() % MAX_RAND;
    printf(" %d,",vec[i]);
	}
}

int main(int argc, char *argv[])
{
    int my_rank;
    int num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   // grab this process's rank
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // grab the total num of processes
    MPI_Status status;

    // create vector
    int vec[N];

    // chunksize
   // int chunk_size = N / num_procs;

    if (!my_rank)
    {

        // settings for P0
        printf("Number of processes: %d\n", num_procs);
        printf("N: %d\n", N);
 //       printf("Chunk size: %d\n", chunk_size);
        srand(time(NULL));
        init_vec(vec, N);

        /*Constants*/
        // time variables
        double start_time;
        double stop_time_odd;
        double stop_time_even;

        // Send this to Team
        start_time = MPI_Wtime();        // can use this function to grab a
                                         // timestamp (in seconds)
        MPI_Send(&vec, N, MPI_INT, 1, 1, // send with tag team1
                 MPI_COMM_WORLD);        // Send to P1
        MPI_Send(&vec, N, MPI_INT, 2, 2, // send with tag team2
                 MPI_COMM_WORLD);        // Send to P2

        // Wait until it receive
        MPI_Recv(&vec, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status); // receive vec from which team first
//	printf("\ntesting1");
        // Compare time and show result
        if (status.MPI_TAG == 1)
        {
            stop_time_odd = MPI_Wtime();
        }
        else  if (status.MPI_TAG == 2)
        {
            stop_time_even = MPI_Wtime();
        }

	MPI_Recv(&vec, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        // Compare time and show result
        if (status.MPI_TAG == 1)
        {
            stop_time_odd = MPI_Wtime();
        }
        else  if (status.MPI_TAG == 2)
        {
            stop_time_even = MPI_Wtime();
        }

        printf("\nOdd team time: %f\n", stop_time_odd - start_time);
        printf("\nEven team time: %f\n", stop_time_even - start_time);
        if (stop_time_odd - start_time < stop_time_even - start_time)
        {
            printf("\n Odd team Wins!\n");
        }
        else
        {
            printf("\n Even team Wins!\n");
        }
    }
    else
    {
        // ELSE IF TEAM == odd
        MPI_Status status;
        int Tag;
        MPI_Recv(vec, N, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        Tag = status.MPI_TAG;
        int next_rank = ((my_rank + 2) <=( num_procs - 1)) ?( my_rank + 2) : 0;
       printf("\nmyrank: %d, my next rank : %d \n",my_rank, next_rank);
	 MPI_Send(&vec, N, MPI_INT, next_rank, Tag, MPI_COMM_WORLD);

        // recv vec
        // for(n){
        // if((myrank+2)<=p-1
        // send to my_rank+1
        // else
        //  send to 0
        // }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
