#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
void bfs(int **graph, size_t num_cities, int start) {

    bool* visitedCities = (bool*)malloc(sizeof(bool) * num_cities);
    int* distanceBetween = (int*)malloc(sizeof(int) * num_cities);
    
    if (!visitedCities || !distanceBetween) {
        printf("Memory allocation error.\n");
        free(visitedCities);
        free(distanceBetween);
        return;
    }

    for (size_t i = 0; i < num_cities; ++i) {
        visitedCities[i] = false;
        distanceBetween[i] = 0;
    }

    visitedCities[start] = true;
    Queue* q = createQueue(num_cities);
    if (!q) {
        printf("Memory alloction error.\n");
        free(visitedCities);
        free(distanceBetween);
        free(q);
        return;
    }
    enqueue(q, start);
    // Enqueue current node after marking it as visited (starting with source)

    while(!isEmpty(q)) {
        // print q
        int city = dequeue(q);
        // visited start
        for (size_t i = 0; i < num_cities; ++i) {
            if (graph[city][i] && !visitedCities[i]) { // if a city exists that we havent visited
                visitedCities[i] = true; // visit it, and enqueue it
                distanceBetween[i] = distanceBetween[city] + 1;
                enqueue(q, i);
            }
        }
    }
    // find minimum in distanceBetween (shortest Path length from source)
    int shortestPathDestination = num_cities-1;
    if (!visitedCities[shortestPathDestination]) {
        printf("-1\n");
    } else {
        printf("%d\n", distanceBetween[shortestPathDestination]);
    }
    free(visitedCities);
    free(distanceBetween);
    freeQueue(q);
    return;
}
