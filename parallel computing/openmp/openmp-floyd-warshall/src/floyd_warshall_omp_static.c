#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#define N 6
void floyd(int A[N][N], int n)//, int schedule_chunk, char *schedule_mode)
{
  // #pragma omp parallel for schedule(static, 64)
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
 for (int i = 0; i < N; i++) {
      //  printf("Row %d: %p\n", i, (void*)A[i]);  // Printing the address of each row
        for (int j = 0; j < N; j++) {
            printf("A[%d][%d]: %d\n", i, j, A[i][j]);  // Printing the address of each element
        }
    }
}

//void init_A(int** A, int n)
//{
 //   for (int i = 0; i < n; i++)
  //  {
   //     for (int j = 0; j < n; j++)
    //    {
     //       if (i == j)
      //      {
       //         A[i][j] = 0;
        //    }
         //   else
          //  {
           //     A[i][j] = (rand() % 10) + 1; // Random weights for edges
           // }
      //  }
   // }
//}

int main()
{
    int n_iter[] = {5};  // Sizes of matrices to test
    int num_threads[] = {8}; // Number of threads to test
   // int schedule_chunk[] = {1, 2, 4, 8, 16, 32, 64}; // Chunk sizes
    //char *schedule_mode[] = {"static", "dynamic", "guided"}; // Scheduling modes

   // for (int q = 0; q < 1; q++)  // Loop over different matrix sizes
  //  {
      //  int n = n_iter[q];
     //   int **A = (int **)malloc(n * sizeof(int *));
    //    for (int i = 0; i < n; i++)
   //     {
  //          A[i] = (int *)malloc(n * sizeof(int));
 //       }
//
      //  init_A(A, n); // Initialize the matrix
	int A[N][N] = { {0,   2,  5,  INT_MAX, INT_MAX,INT_MAX},
                        {INT_MAX, 0,  7,  1,   INT_MAX,8},
                        {INT_MAX, INT_MAX, 0,  4, INT_MAX,INT_MAX},
                        {INT_MAX, INT_MAX, INT_MAX,  0,   3,INT_MAX},
                        {INT_MAX, INT_MAX, 2, INT_MAX,   0,3},
                        {INT_MAX, 5, INT_MAX, 2,  4, 0} };

for (int i = 0; i < N; i++) {
      //  printf("Row %d: %p\n", i, (void*)A[i]);  // Printing the address of each row
        for (int j = 0; j < N; j++) { 
            printf("A[%d][%d]: %d\n", i, j, A[i][j]);       
} 
}
for (int t = 0; t < 1; t++)  // Loop over number of threads
        {
            omp_set_num_threads(num_threads[t]); // Set number of threads

            // Loop over scheduling modes
            //for (int a = 0; a < 3; a++) {
                // Loop over chunk sizes
              //  for (int p = 0; p < 7; p++) {
                    double start = omp_get_wtime(); // Start timing
int n = N;
                    floyd(A, n);//, schedule_chunk[p], schedule_mode[a]);  // Perform Floyd's algorithm

                    double end = omp_get_wtime(); // End timing

                    printf("Matrix size: %d x %d, Threads: %d, Execution time: %.5f seconds\n", 
                           n, n, num_threads[t], end - start);
               // }
           // }
        }
 for (int i = 0; i < N; i++) {
      //  printf("Row %d: %p\n", i, (void*)A[i]);  // Printing the address of each row
        for (int j = 0; j < N; j++) {
            printf("A[%d][%d]: %d\n", i, j, A[i][j]);  // Printing the address of each element
        }
    }
//int n=6;
        // Free the matrix memory
    //    for (int i = 0; i < n; i++)
   //     {
  //          free(A[i]);
 //       }
//        free(A);
   // }

    return 0;
}

