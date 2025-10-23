#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void sieve_of_eratosthenes(int n, int t) {
    // Create a boolean array of size n+1 to mark numbers
    int *is_prime = (int *)malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) {
        is_prime[i] = 1; // Initially assume all numbers are prime
    }
    is_prime[0] = is_prime[1] = 0; // 0 and 1 are not prime numbers

    // OpenMP parallel region
    #pragma omp parallel for num_threads(t) schedule(dynamic)
    for (int k = 2; k * k <= n; k++) {
        if (is_prime[k]) {
            // Mark multiples of k as non-prime
            for (int multiple = k * k; multiple <= n; multiple += k) {
                is_prime[multiple] = 0;
            }
        }
    }

    // Print the primes
    printf("Primes up to %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Free the memory
    free(is_prime);
}

int main() {
    int t = 8; // Number of threads, starting with 8
    int n = 100000; // Upper limit of the range

    double start_time, end_time;

    // Record execution time
    start_time = omp_get_wtime();
    sieve_of_eratosthenes(n, t);
    end_time = omp_get_wtime();

    printf("Execution time for t = %d, n = %d: %.5f seconds\n", t, n, end_time - start_time);

    return 0;
}

