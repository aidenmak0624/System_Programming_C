#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#define N (1<<4)
// 1.set up
// 2.init_vec()
// 3. scatter()


int main(int argc, char *argv[]){

// int MPI_Scatterv(
//     void* sendbuf,       // Send buffer (data to scatter)
//     int* sendcounts,     // Array specifying the number of elements sent to each process
//     int* displs,         // Array specifying the displacement (starting index) of data for each process
//     MPI_Datatype sendtype,  // Data type of send buffer elements
//     void* recvbuf,       // Receive buffer (where the received data will be stored)
//     int recvcount,       // Number of elements this process will receive
//     MPI_Datatype recvtype,  // Data type of receive buffer elements
//     int root,            // Rank of the root process (sender)
//     MPI_Comm comm        // Communicator (e.g., MPI_COMM_WORLD)
// );
    int my_rank;
    int num_proc; // size of the comm

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    const int stride = 2;

    int * sendBuffer = (int*)malloc(num_proc*stride*sizeof(int));
    int * sendcounts = (int*)malloc(num_proc*sizeof(int));
    int * displs = (int*)malloc(num_proc*sizeof(int));

    int receiveBuffer[1];

   
       printf("\nProcess %d before scatterv():\n ",my_rank);
        printf("%d ", receiveBuffer[0]);
        
        printf("\n");


    if( my_rank == 0){

        printf("Number of processes: %d\n", num_proc);
        printf("N: %d\n", N);
        
        for(int i = 0; i< num_proc; i++){
            displs[i] = i*stride;
            sendcounts[i] = 1;
printf("displs[%d]: %d ",i,displs[i]);
        }

        for(int i = 0; i< num_proc* stride; i++){
            sendBuffer[i] = i;
        }
//print the array
        printf("\nProcess 0 initialized sendBuffer:\n ");
        for (int i = 0; i < num_proc*stride; i++)
        {
        printf("%d ", sendBuffer[i]);
        }
        printf("\n");
//scatterv()
}

    MPI_Scatterv( sendBuffer, sendcounts, displs, MPI_INT, receiveBuffer, 1, MPI_INT, 0, MPI_COMM_WORLD);    



   
       printf("\nProcess %d after scatterv():\n ",my_rank);
       
        printf("%d ", receiveBuffer[0]);
        
        printf("\n");


	sleep(1);


	//Gatherv()
if(my_rank==0)
printf("\n\n\nstart Gatherv()\n");
	receiveBuffer[0] = receiveBuffer[0] + 10;


	int sendCount = my_rank+1; 
	int * sendBuffer2 = (int*) malloc(sendCount*sizeof(int));
	 printf("\nProcess %d after init for Gatherv():\n ",my_rank);
	
	for (int i = 0; i < sendCount; i++) {
        sendBuffer2[i] = my_rank * 10 + i; 
        printf("%d",sendBuffer2[i]);
	 }
	

    int* receiveCounts = NULL;
    int* displs2 = NULL;
    int total_elements = 0;

    // Only root process initializes recvcounts and displs
    if (my_rank == 0) {
        receiveCounts = malloc(num_proc * sizeof(int));
        displs2 = malloc(num_proc * sizeof(int));

        // Calculate the number of elements each process sends and the displacement in the recv buffer
        for (int i = 0; i < num_proc; i++) {
            receiveCounts[i] = i + 1;  // Process i sends i+1 elements
            displs2[i] = total_elements;  // Displacement for process i
            total_elements += receiveCounts[i];
        }
    }

    // Root process needs a receive buffer to gather all the data
    int* receiveBuffer2 = NULL;
    if (my_rank == 0) {
        receiveBuffer2 = malloc(total_elements * sizeof(int));  // Allocate enough space to gather all data
    }
	MPI_Gatherv(sendBuffer2, sendCount, MPI_INT, receiveBuffer2, receiveCounts, displs2, MPI_INT, 0, MPI_COMM_WORLD);


    if (my_rank == 0) {
        printf("Root process received: ");
        for (int i = 0; i < total_elements; i++) {
            printf("%d ", receiveBuffer2[i]);
        }
        printf("\n");
        free(receiveBuffer2);
        free(receiveCounts);
        free(displs);
    }

    free(sendBuffer2);

	
	sleep(2);
if(my_rank==0)
	printf("\n\n\nStart MPI_Reduce_Scatter()\n");

int *sendBuffer3 =(int*) malloc(num_proc*sizeof(int));

int receiveBuffer3[1];

int *receiveCount3 = (int*) malloc(num_proc*sizeof(int));

	for(int i= 0; i <num_proc;i++){
		sendBuffer3[i] = my_rank+i+1;
		receiveCount3[i] = 1;
	
	}


 printf("\nProcess %d,MPI_Reduce_Scatter()- local data: ", my_rank);
    for (int i = 0; i < num_proc; i++) {
        printf("%d ", sendBuffer3[i]);
    }
    printf("\n");


  MPI_Reduce_scatter(sendBuffer3, receiveBuffer3, receiveCount3, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Print the received results after reduce scatter
    printf("\nProcess %d, received data: ", my_rank);
    //for (int i = 0; i < 1; i++) {
        printf("%d ", receiveBuffer3[0]);
   // }
    printf("\n");
//int MPI_Reduce_scatter(const void* sendbuf, void* recvbuf, const int recvcounts[], 
  //                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)

//print the local array
     
    //gatherv
    //reduce>?
    MPI_Finalize();
free(sendBuffer);
free(sendcounts);
free(displs);

free(sendBuffer3);
free(receiveCount3);
    return EXIT_SUCCESS;
}
