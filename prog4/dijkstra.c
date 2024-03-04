#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int minimumDistance(int distance[], bool visited[], int numCities) {

    int minimum = INT_MAX;
    int minimumIndex;

    for (int i = 0; i < numCities; ++i) {
        if (!visited[i] && distance[i] <= minimum) {
            minimum = distance[i]; // choose shorter unvisited route
            minimumIndex = i;
        }
    }
    return minimumIndex;
}
// Function to perform Dijkstra's algorithm on a graph represented as an adjacency matrix
void dijkstra(int **graph, int num_cities, int src) {
    int distanceBetween[num_cities];
    bool visitedCities[num_cities];

    for (int i = 0; i < num_cities; ++i) {
        distanceBetween[i] = INT_MAX;
        visitedCities[i] = false;
    }

    distanceBetween[src] = 0; // Source to itself is always 0
    
    for (int i = 0; i < num_cities; ++i) {
        // Choosing vertex closest to current location, given it has not been already visited.
        int city = minimumDistance(distanceBetween, visitedCities, num_cities); 

        // mark city as visited
        visitedCities[city] = true;

        for (int j = 0; j < num_cities; ++j) {
            // Update distance to current vertex (city) if:
            // City has not been visited,
            // There is an edge from "city" to "j"
            // Weight from src to "j" is smaller than distanceBetween[j]
            if (!visitedCities[j] 
                && graph[city][j] 
                && distanceBetween[city] != INT_MAX 
                && (distanceBetween[city] + graph[city][j]) < distanceBetween[j]) {
                    distanceBetween[j] = distanceBetween[city] + graph[city][j]; // Relaxation step
                }
        }
    }

    if (!visitedCities[num_cities-1]) { // if there is no path to the destination
        printf("-1\n");
    } else {
        printf("%d\n", distanceBetween[num_cities-1]);
    }
    return;
}

