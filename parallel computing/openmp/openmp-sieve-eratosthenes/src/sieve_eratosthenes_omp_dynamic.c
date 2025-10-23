#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void sieve_of_eratosthenes(int n,int t){
    //prime = 1, not primr =0
    int *is_prime_array = (int *)malloc((n + 1) * sizeof(int));

    for (int i = 0; i <= n; i++) {
//printf("hi");    
    is_prime_array[i] = 1;
//printf("%d",is_prime_array[i]); 
    }
    is_prime_array[0] = 0;
    is_prime_array[1] = 0;


#pragma omp parallel num_threads(t)
    {
        #pragma omp for schedule(dynamic, 500)
    for(int  j= 2 ; j< n; j++){
//	printf("HE1");
if(j*j<n){
        if(is_prime_array[j]){
            for (int multiple = j * j; multiple <= n; multiple += j) {
                is_prime_array[multiple] = 0;
        //printf("HEHE");  
	  }
        }
}
    }
}
    printf("Primes up to %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (is_prime_array[i]==1) {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Free the memory
    free(is_prime_array);

}

int main() {
    int t = 8; // Number of threads, starting with 8
    int n = 80000000; // Upper limit of the range

    double start_time, end_time;

    // Record execution time
    start_time = omp_get_wtime();
    sieve_of_eratosthenes(n,t);
    end_time = omp_get_wtime();

    printf("Execution time for t = %d, n = %d: %.5f seconds\n", t, n, end_time - start_time);

    return 0;
}
