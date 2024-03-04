#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
void printNumber( void *num )
{
	int *i = num;		// "casting" void * to int *
	printf( "%d    ", *i );	// print what i is pointing to
}

void printString( void *str )
{
	char *s = str;		// "casting" void * to char *
	printf( "%s    ", s );	// print string starting from s
}

int main(int argc, char **argv){


	void *data;
	int  value;
	int  num1 = 123;
	int  num2 = 321;
	char st1[100] = "hello";
	char st2[100] = "good bye";

	// Declare a new num list

	ListPtr L = createList( printNumber );
	 
	// Perform desired operation on list
	
	appendList( L, &num2 );
	appendList( L, &num1 );

	int len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	destroyList( &L );

	printf( "num list after destroying it is: " );
	printList( L );
	printf( "-- \n" );

	L = createList( printNumber );	// why is this line needed?

	appendList( L, &num1 );
	appendList( L, &num2 );
	appendList( L, &num1 );

	len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	for( int i=0; i<len; i++ ) {
	    data = getList( L, i );
	    if (data != NULL) {
		value = *(int *)data;	// contents of data after casting to int *
		printf( "%dth element of list is %d\n", i, value );
	    }
	}

	len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	// Declare a new list

	printf( "\nReuse L for strings\n" );

	destroyList(&L);
	L = createList( printString );	// don't i need to call destroyList(L) 1st?

	appendList( L, st1 );
	appendList( L, st2 );

	len = lengthList( L );
	printf( "string list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	destroyList(&L);
	L = createList(printNumber);
	
	// destroyList function testing

	int n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;
	appendList(L, &n1);
	appendList(L, &n2);
	appendList(L, &n3);
	appendList(L, &n4);
	appendList(L, &n5);
	printList(L);
	printf("\n\n");

	// List = [1, 2, 3, 4, 5]
	// TODO:  add some code to test your deleteList()
	
	// Attempt to deleteList at negative index
	assert(deleteList(L, -1) == NULL); 
	// Expected output: NULL


	// Attempt to deleteList at head index
	printf("Testing deletion of head node (index 0)...\n");
	printf("Before deletion: ");
	printList(L);
	printf("\n");

	assert(deleteList(L, 0) == &n1);
	printf("After deletion: ");
	printList(L);
	printf("\n\n");

	
	// Attempt to deleteList at "middle" of list
	printf("Testing deletion of 'middle' node (index 3)...\n");
	printf("Before deletion: ");
	printList(L);
	printf("\n");

	assert(deleteList(L, 3) == &n5);
	printf("After deletion: ");
	printList(L);
	printf("\n\n");

	
	// Attempt to deleteList at index past list
	printf("Testing deletion at index past the edge of the list...\n");
	assert(deleteList(L, 100) == NULL);
	printf("Attempt to delete node at invalid index unsuccessful. (return value: null)\n");
	printf("\n");
	// Expected output: (null) since L->length == 3 here.
	

	// Attempt to deleteList with L = NULL at valid index
	printf("Testing deletion inside of null list...\n");
	destroyList(&L);
	L = NULL;
	assert(deleteList(L, 1) == NULL);
	printf("Attempt to delete note from null list unsuccessful. (return value: null)\n");
	printf("\n");
	// Expected output: (null) since L points to NULL (no list)

	printf("Testing complete\n");
	return 1;
}	