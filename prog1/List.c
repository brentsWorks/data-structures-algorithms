#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include <assert.h>



ListPtr createList( void (*dataPrinter)(void *data) ) {
	// Your implementation of createList()
	ListPtr newList = (ListObj*)malloc(sizeof(ListObj));
	
	if (newList == NULL) {
		return NULL;
	}

	newList->head = NULL;
	newList->dataPrinter = dataPrinter;
	newList->length = 0;

	return newList;
}

void destroyList(ListPtr *pL) {
	// Your implementation of destroyList()
	if ( (*pL)->head == NULL)
		return;
	
	NodePtr ptr = (*pL)->head;
	NodePtr tmp;

	while (ptr != NULL) {
		tmp = ptr->next;
		free(ptr);
		ptr = tmp;
	}

	free(*pL);
	(*pL) = NULL;
	return;
}

int lengthList(ListPtr L) {
	// Your implementation of lengthList()
	if (L->length)
		return L->length;
	return -1;
}

void printList(ListPtr L) {
	// Your implementation of printList()
	if (L == NULL)
		return;

	NodePtr ptr = L->head;
	while (ptr != NULL) { // Call function to print data while traversing list
		L->dataPrinter(ptr->data);
		ptr = ptr->next;
	}
	return;
}

void *getList(ListPtr L, int i) {
	// Your implementation of getList()
	if (L == NULL) return NULL;

	NodePtr ptr = L->head; // create ptr to head node of given list

	if (i == 0)
		return ptr->data; // return data right away if index is at head node
	else {
		for (size_t count = 0; count < i-1; ++count) { // else traverse until given index reached
			ptr = ptr->next;
		}
		return ptr->data; 
	}
	return NULL;
}

void *deleteList(ListPtr L, int i) {
	// Your implementation of deleteList()
	if (L == NULL || L->head == NULL || i >= L->length || i < 0)  // Protective clause 
		return NULL;

    void *data = NULL;
    NodePtr prev = NULL;
    NodePtr current = L->head;

    if (i == 0) { // If entry to be deleted is at head
        L->head = L->head->next;
        data = current->data;
        free(current);
    } else { // Else, entry to be deleted is past head
        for (size_t count = 0; count != i; ++count) {
            prev = current;
            current = current->next;
        } 
        if (current != NULL) { // After for loop, we're at correct node and must verify != NULL.
            prev->next = current->next;
            data = current->data;
            free(current);
        }
    }
    --L->length;
    return data;
}

bool appendList(ListPtr L, void *data) {

	if (L->head == NULL) {
		L->head = (NodeObj*)malloc(sizeof(NodeObj));
		L->head->data = data;
		L->head->next = NULL;
		++L->length;
		return true;
	}
	
	// Initialize pointer to L->head, traverse ptr->next until NULL
	else {
		NodePtr ptr = L->head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
			// Once ptr->next = NULL, we know we are at the last node
		ptr->next = (NodeObj*)malloc(sizeof(NodeObj));
		ptr->next->data = data;
		ptr->next->next = NULL;
		++L->length;
		return true;
	}
	return false;
	
}
