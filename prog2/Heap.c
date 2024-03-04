#ifndef __HEAP_H_
#define __HEAP_H_
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DO NOT MAKE CHANGES TO THIS FILE

//----------------------------------------------------
// Heap.h
// Header file for Heap
// Heap implementation using an array of type persons (defined below)
// ---------------------------------------------------

typedef struct person
{
    int age;
    char *name;
    char *type;
    char *destinations;
} Person;

typedef struct Heap
{
    Person *arr;
    int size;
    int capacity;
} Heap;

// ------------------------ Heap Property maintainers ----------------------------
// Used to maintain the heap structure top-down
void heapify(Heap *h, int i)
{
    int l = left(i);
    int r = right(i);
    int largest;

    if (l <= h->size && h->arr[l].age > h->arr[i].age) {
        largest = l;
    } else largest = i;

    if (r <= h->size && h->arr[r].age > h->arr[largest].age) {
        largest = r;
    }

    if (largest != i) {
        Person tmp = h->arr[i];
        h->arr[i] = h->arr[largest];
        h->arr[largest] = tmp;
        // swap A[i] with A[largest]
        heapify(h, largest);
    }
  
    return;
}

//------------------------- Constructor-Destructor ------------------------------

// Constructor to initialize heap from an array of Persons, arr, of capacity, capacity, and size, size.
Heap *initializeHeap(Person *arr, int capacity, int size)
{
    Heap *newHeap = (Heap*)malloc(sizeof(Heap));
    if (newHeap == NULL) {
        printf("Memory allocation failed for new Heap.\n");
        return NULL;
    }

    newHeap->arr = (Person*)malloc(sizeof(Person) * (capacity+1));
    if (newHeap->arr == NULL) {
        printf("Memory allocation failed for Heap array.\n");
        return NULL;
    }

    if (arr != NULL) {
        for (int i = 1; i <= size; ++i) {
            newHeap->arr[i].age = arr[i-1].age;
            newHeap->arr[i].destinations = strdup(arr[i-1].destinations);
            newHeap->arr[i].name = strdup(arr[i-1].name);
            newHeap->arr[i].type = strdup(arr[i-1].type);
        }
    }
    
    newHeap->capacity = capacity;
    newHeap->size = size;
    for (int i = (newHeap->size)/2; i >= 1; --i) {
        heapify(newHeap, i);
    }
    return newHeap;
}

// Destructor to free memory of the heap, and then free the memory occupied by the heap pointer.
void destructHeap(Heap **h)
{
    if (h == NULL) return;
    // destroy arr in heap (each person in arr)
    // destroy heap itself
    for (int i = 1; i < (*h)->size; ++i) {
        free((*h)->arr[i].destinations);
        free((*h)->arr[i].type);
        free((*h)->arr[i].name);
    }
    free((*h)->arr); // free array itself
    free((*h)); // free heap
    return;
}


// -------------------------------- Helper Functions -----------------------------------
// return the array index of the parent node
int parent(int i)
{
    return (i / 2);
}

// return the array index of the left child
int left(int i)
{
    return (2 * i);
}

// return the array index of the right child
int right(int i)
{
    return (2 * i) + 1;
}

// Prints the Names and ages of the people in the heap.
void printHeap(Heap *h)
{
    if (h == NULL || h->arr == NULL) return;

    for (int i = 1; i <= h->size; ++i) {
        printf("Name: %s | Age: %d\n", h->arr[i].name, h->arr[i].age);
    }
    return;
}

// Return the length of the array in the heap
int len(Heap *h)
{
    if (h == NULL)
        return 0;
    return h->size;
}

// return the capacity of the heap
int size(Heap *h)
{
    if (h == NULL)
        return 0;
    return h->capacity;
}

#endif // __HEAP_H_
