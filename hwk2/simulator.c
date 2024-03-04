#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<limits.h>
#include"Queue.h"
#include"List.h"


typedef struct Station {
	char* name;
	int quantity;
	Queue* patrons;
} Station;

void printString( void *str )
{
	char *s = str;		// "casting" void * to char *
	printf( "%s    ", s );	// print string starting from s
	return;
}

void stationPrinter(ListPtr stationList) {
	NodePtr ptr = stationList->head;
	while (ptr != NULL) { // While not at end of station list, traverse through stations
		Station* ptrStation = ptr->data;
		printf("%s Station\n", ptrStation->name);
		for (size_t i = 0; i < ptrStation->patrons->size; ++i) { // For length of queue, print each name in queue at said station
			printf("'%s' => ", ptrStation->patrons->array[i]); 
		}
		printf("\n");
		ptr = ptr->next; // advance to next station to continue printing
	}
	return;
}

bool validKey(int option) {
	if (option > 0 && option <= 7) return true;
	return false;
}

bool validStation(ListPtr stationList, char* station) {
	if (station == NULL || stationList == NULL) {
		return false; 
	}

	NodePtr ptr = stationList->head;
	while (ptr != NULL) {
		Station* ptrStation = ptr->data; 
		if (ptrStation != NULL && ptrStation->name != NULL && strcmp(ptrStation->name, station) == 0) {
			return true;
		}  // iterate stations_list while not at end, if we find a matching station then set bool and break loop
		ptr = ptr->next; 
	}
	// kick out to while loop input again if station entered is invalid
	return false;

}

Station* findStation(NodePtr ptr, char* stationName) {
	// Helper function to findStation given that we know stationName is a valid station.
	// ptr points to head of stations_list
	if (stationName == NULL) return NULL;

	while (ptr != NULL) {
		Station* desiredStation = ptr->data; 
		if (desiredStation != NULL && desiredStation->name != NULL && strcmp(desiredStation->name, stationName) == 0) {
			return desiredStation;
		}  // iterate stations_list while not at end, if we find a matching station set station* and return
		ptr = ptr->next; 
	}
	return NULL;
}
void parseCommand(int option, ListPtr stationList, ListPtr peopleList) {
	char* patronName = NULL;
	char* stationName = NULL;
	char input[128];
	if (stationList == NULL || peopleList == NULL) return;

	NodePtr peoplePtr = peopleList->head; // node in people_list
	NodePtr ptr = stationList->head;  // node in stations_list
	Station* ptrStation = ptr->data; // struct object
	// else we have a valid command, so we parse it using our option key now

	switch (option) {
		case 1:
			// Format: Alex Dessert

			patronName = (char*)malloc(sizeof(char) * 128);
			stationName = (char*)malloc(sizeof(char) * 128);
			if (patronName == NULL || stationName == NULL) {
				printf("Memory allocation failed.\n");
				return;
			}
			scanf("%s %s", patronName, stationName);

			if (validStation(stationList, stationName)) { // if station is valid, find where it is and return ptr
				ptrStation = findStation(ptr, stationName); 
				enqueue(ptrStation->patrons, patronName);  // enqueue at given station (findStation return value)
			} else {
				printf("Error: Station %s does not exist. Please enter a valid command and station.\n", stationName);
			}

			free(patronName);
			free(stationName);
			break;
		case 2:
			// Format: Dessert 20
			stationName = (char*)malloc(sizeof(char) * 128);
			int dishCount;
			scanf("%s %d", stationName, &dishCount);
			
			if (validStation(stationList, stationName)) {
				ptrStation = findStation(ptr, stationName);
				ptrStation->quantity += dishCount;
			}  else {// restock if valid station
				printf("Error: Station %s does not exist. Please enter a valid command and station.\n", stationName);
			}
			free(stationName);
			break;
		case 3:
			// Format: Dessert Alex
			patronName = (char*)malloc(sizeof(char) * 128);
			stationName = (char*)malloc(sizeof(char) * 128);

			fgets(input, sizeof(input), stdin);
			if (sscanf(input, "%s %s", patronName, stationName) == 2) {
				if (validStation(stationList, stationName)) {
					ptrStation = findStation(ptr, stationName);
					if (front(ptrStation->patrons) != patronName) {
						printf("Patron is either not in the given queue or not at the front of this line.\n");
						free(patronName);
						free(stationName);
						return;
					} 	// Patron leaves queue if they're at the front. otherwise if they arent, or don't belong in that queue, print error
					dequeue(ptrStation->patrons);
				}
			}
			free(patronName);
			free(stationName);
			break;
		case 4:
			// Format: Alex
			patronName = (char*)malloc(sizeof(char) * 128);
			scanf("%s", patronName);
			// If patron in people_list, remove them (deleteList)
			
			int position = 0;
			while (peoplePtr != NULL) { // traverse list while not null, if name found then delete it
				if (strcmp(peoplePtr->data, patronName) == 0) {
					deleteList(peopleList, position);
					free(patronName);
					return;
				}
				peoplePtr = peoplePtr->next; // else keep looking and increment index
				++position;
			}
			ptr = stationList->head;
			ptrStation = ptr->data;

			while (ptr != NULL) {
				if (front(ptrStation->patrons) == patronName) { // else, traverse through station queues one by one checking front(ptrStation->patrons)
					dequeue(ptrStation->patrons);
					free(patronName);
					return;
				}
				ptr = ptr->next;
				ptrStation = ptr->data;
			}
			printf("Error: Patron is not currently at the front of any queue in the dining hall.\n");
			free(patronName);
			break;
		case 5:
			// Format: Dessert
			stationName = (char*)malloc(sizeof(char) * 128);
			if (stationName == NULL) {
				printf("Memory allocation for stationName failed.\n");
			}
			scanf("%s\n", stationName);
			if (!validStation(stationList, stationName)) { // invalid station so just return
				free(stationName);
				return;
			} 
			
			ptrStation = findStation(ptr, stationName);
			// Serve dessert to everyone in queue as possible, til out of dessert or everyone served
			while (ptrStation->quantity > 0 && ptrStation->patrons->size > 0) {
				// For every person served, append them to people_list and decrease quantity by 1 of available dish
				patronName = strdup(front(ptrStation->patrons));
				dequeue(ptrStation->patrons);
				appendList(peopleList, patronName);
				free(patronName);
				--ptrStation->quantity;
			}
			// If station does not have any dish while this option is called, and there are people in its queue, 
			if (ptrStation->patrons->size > 0) { // station dish quantity must be 0
				printf("Apologies for the inconvenience. Please wait for the queue to be filled.\n");
			}
			free(stationName);
			break;
		case 6:
			// Format: filename
			;char fileName[128];
			scanf("%s", fileName);
			FILE* fptr = fopen(fileName, "w");
			if (!fptr) {
				printf("Error opening input file.\n");
				return;
			}

			// Point to, stations_list, print queue array in node, traverse to next node, do the same until null
			// ptr at stations_list head
			    // Check if ptr is NULL
			if (ptr == NULL) {
				printf("Error: Station list is empty.\n");
				fclose(fptr);
				return;
			}

			while (ptr != NULL) {
				ptrStation = ptr->data;
				// Check if ptrStation is NULL
				if (ptrStation == NULL) {
					printf("Error: Station data is missing.\n");
					fclose(fptr);
					return;
				}

				for (size_t i = 0; i < ptrStation->patrons->size; ++i) {
					fprintf(fptr, "%s\n", ptrStation->patrons->array[i]);
				}
				fprintf(fptr, "\n\n");
				ptr = ptr->next;
			}

			while (peoplePtr != NULL) { // After writing the names in all the queues, write the contents of people_list to filename, starting from the head of the list
				fprintf(fptr, "%s => ", peoplePtr->data);
				peoplePtr = peoplePtr->next;
			}
			fprintf(fptr, "\n");
			fclose(fptr);
			return;
		default:
			printf("Error: Switch case parameter out of range.\n");
			 printf("Error: Switch case parameter out of range.\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			return;
	}	
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	return;
}

int main(int argc, char **argv){
	//code to create queue
	int numCounters = 0, totalPeople = 0, dishQuantity = 0;
	char personName[128];
	char dishType[128];		
	Station* dish = NULL;
	ListPtr stations_list = createList(NULL);

	scanf("%d\n", &numCounters);
	for (int i = 0; i < numCounters; ++i) {
		scanf("%s %d\n", dishType, &dishQuantity);

		dish = (Station*)malloc(sizeof(Station)); 		// initialize dish (Station*) to Station node
		if (dish == NULL) {
			printf("Memory allocation for new Station failed.\n");
			return 1;
		}
		
		dish->name = strdup(dishType);
		dish->quantity = dishQuantity;
		dish->patrons = initializeQueue(0, NULL);
		appendList(stations_list, dish);

	}
	dish = NULL;

	char fileName[64];
	scanf("%s\n", fileName);
	FILE *fptr = fopen(fileName, "r");
	if (!fptr) {
		printf("Error opening input file.\n");
		return 1;
	}

	fscanf(fptr, "%d", &totalPeople);

	NodePtr stationPtr;
	while (fscanf(fptr, "%s %s", personName, dishType) != EOF) { // Read in line by line while not at EOF
		stationPtr = stations_list->head; // initialize ptr to head node of station list
		dish = stationPtr->data; // initialize station ptr to first station

		while (strcmp(dish->name, dishType) != 0 && stationPtr != NULL) { // While we are not at correct station to enqueue person at, 
			stationPtr = stationPtr->next;			// advance to next station
			dish = stationPtr->data;
		}
		// Once loop exits, we are at correct station to enqueue at
		enqueue(dish->patrons, personName);  // then we enqueue the person who wants to eat from said station
	
	} 
	fclose(fptr);
	//stationPrinter(stations_list); 

	ListPtr people_list = createList(printString); 
	// SECOND PART OF PROGRAM
	int optionKey;

	while (true) {
		int c;
		int valid = 0;
		//char tempChar;
		//valid = scanf("%c", &tempChar);
		//printf("|%c|\n", tempChar);
		valid = scanf("%d", &optionKey); // will be 1 if successful

		if (valid == EOF) { 
			printf("End of input reached. Freeing memory and exiting..\n");
			break;
		}

		if (valid != 1) { // if scan was not integer
			printf("Error: Invalid command. Try inputting a valid integer 1-7.\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}

		if (!validKey(optionKey)) {
			printf("Error: Invalid command. Try inputting a command 1-7\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		if (optionKey == 7) {
			printf("Exiting program... taking a second to free all allocated memory\n");
			break;
		} else {
			// else input is 1-6 so parse it
			parseCommand(optionKey, stations_list, people_list);
			continue;
		}	
		//printf("Error: Invalid command. Try inputting a command 1-7.\n");
	}	
	
	NodePtr peoplePtr = people_list->head;
	while (peoplePtr != NULL) {
		free(peoplePtr->data);
		peoplePtr = peoplePtr->next;
	}
	destroyList(&people_list);

	stationPtr = stations_list->head;
	while (stationPtr != NULL) { // free all queues for each station
		dish = stationPtr->data;
		free(dish->name);
		destructQueue(&dish->patrons); // destruct pointer to queue
		free(dish);
		stationPtr = stationPtr->next;
	}
	destroyList(&stations_list); // free station list

	printf("Memory has been deallocated... terminating program.\n");
	return 0;
}