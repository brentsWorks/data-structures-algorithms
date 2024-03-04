#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Heap.h"
#include "PriorityQueue.h"


int main() {
    Person people[] = {
        {25, "Alice", "TypeA", "DestA"},
        {30, "Bob", "TypeB", "DestB"},
        {22, "Charlie", "TypeC", "DestC"},
        {35, "David", "TypeD", "DestD"},
        {28, "Eve", "TypeE", "DestE"},
    };

    int numPeople = sizeof(people) / sizeof(people[0]);
    Heap* heap = initializeHeap(people, numPeople, numPeople);

    printf("Initial heap:\n");
    printHeap(heap);

    // Test extractMax
    printf("\nExtracting max (oldest person):\n");
    Person* maxPerson = extractMax(heap);
    if (maxPerson) {
        printf("Name: %s, Age: %d\n", maxPerson->name, maxPerson->age);
        free(maxPerson);
    } else {
        printf("Heap is empty.\n");
    }

    Person newPerson = {40, "Frank", "TypeF", "DestF"};
    printf("\nInserting a new person: %s, %d\n", newPerson.name, newPerson.age);
    insert(heap, &newPerson);

    printf("\nUpdated heap after insertion:\n");
    printHeap(heap);

    increasePriority(heap, 3, 50); 

    printf("\nUpdated heap after increasing priority:\n");
    printHeap(heap);

    printf("\nMax heapified: \n");
    for (int i = (numPeople / 2) - 1; i >= 1; i--) {heapify(heap, i);}
    printHeap(heap);
    destructHeap(&heap);
     return 0;
}
