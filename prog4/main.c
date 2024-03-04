
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "bfs.h"
#include "dijkstra.h"

int parseCommand(int, int**, int);
bool isValidCommand(int);

int main() {
    char fileName[64];
    FILE* fptr;

    // read in cityX.in and open it
    scanf("%s\n", fileName);
    fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("Error opening this file %s\n", fileName);
        return -1;
    }

    // initialize graph (adjacency matrix) using city file
    // num_cities       : line 1
    // num_links        : line 2
    // cityX,cityY,time : line 3+
    char buffer[256];

    int num_cities, num_links;
    fscanf(fptr, "%d\n", &num_cities);
    fscanf(fptr, "%d\n", &num_links);
    int** graph = (int**)malloc(sizeof(int*) * num_cities);
    // initialize adjacency list to all zeroes to begin
    for (int i = 0; i < num_cities; ++i) {
        graph[i] = (int*)malloc(sizeof(int) * num_cities);
        for (int j = 0; j < num_cities; ++j) {
            graph[i][j] = 0;
            // edge = non-zero weight between cities i and j
        }
    }

    // initialize adjacency list to represent correct edges (num_cities by num_cities)
    for (int count = 0; count < num_links; ++count) {
        int cityX, cityY, time;
        fscanf(fptr, "%d,%d,%d", &cityX, &cityY, &time);
        graph[cityX][cityY] = time;
    }

    fclose(fptr);
    
    // read in commands
    int retvalue = 0;
    do {
        int command = 0;
        scanf("%d\n", &command);
        // validate command
        if (isValidCommand(command)) { // between 0-3
            retvalue = parseCommand(command, graph, num_cities);
            // parse command, perform action, capture return value 
        } else {
            printf("Invalid command entered. Please enter an integer (0-3).\n");
        }

    } while (retvalue != -1); // breaks out of loop upon command entry '3'

    // free graphs
    for (int i = 0; i < num_cities; ++i) {
        free(graph[i]);
    }
    free(graph);
    return 0;
}

// Helper function to find minimum distance vertex to choose
int newMinimumDistance(int distance[], bool visited[], int numCities) {

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
// AND print all shortest distances from vertex SRC to all other vertices.
void modifiedDijkstra(int **graph, int num_cities, int src) {
    int distanceBetween[num_cities];
    bool visitedCities[num_cities];

    for (int i = 0; i < num_cities; ++i) {
        distanceBetween[i] = INT_MAX;
        visitedCities[i] = false;
    }

    distanceBetween[src] = 0; // Source to itself is always 0
    
    for (int i = 0; i < num_cities; ++i) {
        // Choosing vertex closest to current location, given it has not been already visited.
        int city = newMinimumDistance(distanceBetween, visitedCities, num_cities); 

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

    for (int i = 0; i < num_cities; ++i) {
        if (!visitedCities[i]) {
            printf("Vertex:%d,Distance:-1\n", i);
        } else {
            printf("Vertex:%d,Distance:%d\n", i, distanceBetween[i]);
        }
    }
    return;
}

int parseCommand(int command, int** graph, int numCities) {
    
    switch (command) {
        case 0:
            // BFS Algorithm
            bfs(graph, numCities, 0);
            break;
        case 1:
            // Dijkstra's Algorithm
            // K = -1;
            dijkstra(graph, numCities, 0);
            break;
        case 2:
            // Bonus Challenge: 
            //
            // Display shortest path from 0 to all other vertices 
            //      * print each value in distance array
            //      * modifiedDijkstra with 0 src call
            modifiedDijkstra(graph, numCities, 0);
            // Display shortest path from each vertex to last vertex (n-1)
            //      * calculate shortest path to n-1 
            //        using each vertex as a source once (except last)
            //      * call for loop, iterate to n-1 using iterator as source each time
            //      * dijkstra (i as src)
            for (int i = 0; i < numCities; ++i) {
                printf("Vertex:%d,Distance:", i);
                dijkstra(graph, numCities,i);
            }
            // Vertex:[Vertex Index],Distance:[Distance Value]
            break;
        case 3:
            // Terminate Program
            printf("Goodbye\n");
            return -1;
        default:
            return -1;
    }
}

bool isValidCommand(int value) {
    if (value >= 0 && value <= 3) return true;
    return false;
}