#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include"Queue.h"
// Implementing Queue



//------------------------------------------------------------------------

/*
TODO: implement the initializeQueue - this function instantiates the Queue struct using malloc and sets the values for the fields.
It returns a pointer to the instantiated Queue
*/
Queue* initializeQueue(int n, char** array){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));

    if (newQueue == NULL) {
        return NULL; // Error allocating memory
    }

    newQueue->size = n;
    newQueue->array = (char**)malloc(sizeof(char*) * n);
    // if trouble allocating memory, free what was allocated so far and return null
    if (newQueue->array == NULL) {
        free(newQueue);
        printf("Memory allocation for Queue array failed.\n");
        return NULL;
    }

    // then check if given array is null, if not then go through it and write its elements into queue
    if (array != NULL) {
        for (size_t i = 0; i < n; ++i) {
            if (array[i] != NULL){
                newQueue->array[i] = strdup(array[i]); // = array[i]?
                newQueue->size++;
            } 
            for (size_t j = 0; j < i; ++j) {
                free(newQueue->array[j]);
                free(newQueue);
                return NULL;
            }
            // if element being copied is null (invalid allocation), free elements up to i index in array (allocated elements thus far)
            // free array, and return null 
        }

    }
    return newQueue;
}



//------------------------------------------------------------------------

/*
TODO: implement Queue destructor - this function frees memory allocated to Queue
*/
void destructQueue(Queue** q){
    if (*q == NULL) return;

    for (size_t i = 0; i < (*q)->size; ++i) {
        free( (*q)->array[i] );
    }
    free((*q)->array);
    free(*q);
    (*q) = NULL;
    return;
}


//------------------------------------------------------------------------

/* TODO: implement the 'enqueue' functionality
This function takes in a queue pointer and a username and adds the username to queue
*/

void enqueue(Queue* q, char* name) {

    //Resize queue
    q->array = realloc(q->array, (q->size+1) * sizeof(char*));
    if (q->array == NULL) {
       printf("Memory allocation failed during enqueue.");
       return;
      }

    //Add the new element at the end
    q->array[q->size] = strdup(name);

    //Prevent memory leaks
    if (q->array[q->size] == NULL) {
        printf("Failed to duplicate string during enqueue.");
        return;
    }
    q->size++;
}


//------------------------------------------------------------------------

/* TODO: implement the 'dequeue' functionality
This function takes in a queue pointer and removes element from head of the queue
Print "Empty Queue" if there's not element to remove
*/

void dequeue(Queue* q){
    if(!front(q)) printf("Empty Queue\n");
    
    char** tmp = (char**)malloc(sizeof(char*) * (q->size-1)); // if item at front, create temp array of size x-1
    if (tmp == NULL) {
        printf("Memory re-allocation for dequeue operation failed.\n");
        return;
    }

    for (size_t i = 1; i < q->size; ++i) { // copy original array from index 1 on into temp array @ index i-1 for since tmp is size-1 elements
        tmp[i-1] = strdup(q->array[i]);
        free(q->array[i]);
    }
    free(q->array);
    q->array = (char**)malloc(sizeof(char*) * (q->size-1));     // free original array, reinitialize with size (x-1)

    if (q->array == NULL) {
         printf("Memory re-allocation for dequeue operation failed.\n");
         free(tmp);
         return;
    }
    for (size_t j = 0; j < q->size-1; ++j) { 
        q->array[j] = strdup(tmp[j]);                         // copy elements from temp to reinitialized array
        free(tmp[j]);
    }
    free(tmp);
    --q->size;
    return;
}


//------------------------------------------------------------------------

/* TODO: implement the 'front' functionality
This function takes in a queue pointer and returns the element at the front of the queue without removing it. If the Queue is empty, return NULL
*/

char* front(Queue* q){
    if (q == NULL || q->size == 0) return NULL;
    return (q->array[0]);
}


//------------------------------------------------------------------------

/* TODO: implement the printQueue
This function takes in a queue pointer and prints the contents of the queue in order (head to tail)
*/
void printQueue(Queue* q){
    if (q == NULL) return;

    for (size_t i = 0; i < q->size; ++i) 
        printf("%s\n", q->array[i]);
    return;
}


//------------------------------------------------------------------------

/* TODO: implement the count
This function takes in a queue pointer and returns the 'count' field of the queue
*/
int count(Queue* q){
    if (q == NULL) return 0;

    return q->size;
}

