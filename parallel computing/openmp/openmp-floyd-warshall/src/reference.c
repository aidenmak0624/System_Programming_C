#include <stdio.h>
#include <limits.h>

#define V 6 // Number of vertices in the graph

void floydWarshall(int graph[V][V]) {
    int dist[V][V], i, j, k;

    // Initialize the solution matrix same as input graph matrix.
    // Or we can use the following assignment statements
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    // Add all vertices one by one to the set of intermediate vertices.
    // Process all vertices as a source for each intermediate vertex.
    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                // If vertex k is on the shortest path from i to j, then update the value of dist[i][j]
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // Print the shortest distance matrix
    printf("Shortest distance matrix\n");
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX)
                printf("%7s", "INF");
            else
                printf("%7d ", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    /* Let us create the following graph
              10      0       5       INF    INF
             INF     5       INF     9      4
             INF     INF     0       INF    INF
             INF     INF     2       0       6
             INF     INF     INF    7       0
    */
    int graph[V][V] = { {0,   2,  5,  INT_MAX, INT_MAX,INT_MAX},
                        {INT_MAX, 0,  7,  1,   INT_MAX,8},
                        {INT_MAX, INT_MAX, 0,  INT_MAX, INT_MAX,INT_MAX},
                        {INT_MAX, INT_MAX, 0,  4,   INT_MAX,INT_MAX},
                        {INT_MAX, INT_MAX, 2, INT_MAX,   0,3},
			{INT_MAX, 5, INT_MAX, 2,  4, 0} };

    // Function call
    floydWarshall(graph);
    return 0;
}
