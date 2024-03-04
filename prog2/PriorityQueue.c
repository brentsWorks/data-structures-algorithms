#include "Heap.h"
#include <math.h>
// DO NOT MAKE CHANGES TO THIS FILE

// returns the oldest person in the PQ, without removing that person from the PQ.
Person* getMax(Heap* heap) {
    if (heap == NULL) return NULL;

    if (heap->capacity < 1) {
        printf("Error: Heap Underflow\n");
        return NULL;
    }

    Person* maxPerson = (Person*)malloc(sizeof(Person));
    if (maxPerson == NULL) {
        printf("Error: Memory allocation for deep copy failed.\n");
        return NULL;
    }
    *maxPerson = heap->arr[1];
    return maxPerson;
}

// returns the oldest person in the PQ, and removes them from the PQ. 
Person* extractMax(Heap* heap) {
    if (heap == NULL) return NULL;

    Person* max = getMax(heap); // find max of heap
    heap->arr[1] = heap->arr[heap->size]; // swap first and last (oldest and youngest)
    --heap->size; // drop array capacity by one after extract
    heapify(heap, 1); // re heapify array
    return max;
}

// Updates the age of the person at index, index, in the PQ, to new_priority
void increasePriority(Heap* heap, int index, int new_priority) {
    if (heap == NULL) return;

    if (new_priority < heap->arr[index].age) {
        printf("Priority of new person is smaller than priority of current person.\n");
    }
    heap->arr[index].age = new_priority;

    // while index > 1 and parent(i) priority < i priority
        // swap arr[i] with arr[parent(i)]
        // update information that maps priority queue objects to array indices
        // i = parent(i)

    while (index > 1 && heap->arr[parent(index)].age < heap->arr[index].age) {
        Person tmp = heap->arr[index];
        heap->arr[index] = heap->arr[parent(index)];
        heap->arr[parent(index)] = tmp;
        index = parent(index);
    }
    return;
}

// inserts the person into the PQ
void insert(Heap* heap, Person* person) {
    if (heap == NULL || person == NULL) {
        printf("Heap or person to be inserted is NULL.\n");
        return; 
    }

    if (heap->size == heap->capacity) { // if current array size = to max size, resize to 2x current, continue
        
        heap->arr = realloc(heap->arr, ((heap->size+1) * 2) * sizeof(Person)); 
        heap->capacity *= 2;
    }

    // if given heap && person not null, insert and increment size
    ++heap->size;
    heap->arr[heap->size] = *(person);

    heap->arr[heap->size].destinations = strdup(person->destinations);
    heap->arr[heap->size].type = strdup(person->type);
    heap->arr[heap->size].name = strdup(person->name);
    heap->arr[heap->size].age = person->age;

    int index = heap->size;
    while (index > 1 && heap->arr[parent(index)].age < heap->arr[index].age) {
        Person tmp = heap->arr[index];
        heap->arr[index] = heap->arr[parent(index)];
        heap->arr[parent(index)] = tmp;
        index = parent(index);
    }
    return;
}


