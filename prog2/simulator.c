#include "Heap.h"
#include "PriorityQueue.h"
#include <stdlib.h>

struct Bus {
    char* busName;
    int numBusinessSeats;
    int numEconomySeats;
    char* currStop;
    int indexCurrStop; // Needed in order to find currStop next
    char* validStops;
    Heap* ridingBusiness;
    Heap* ridingEconomy;
} Bus;

struct Stop {
    char* stopName;
    Heap* businessClass;
    Heap* economyClass;
} Stop;

// Helper function to free memory allocated to buses
void freeBuses(struct Bus** listOfBuses, int numBuses);
void freeStops(struct Stop** listOfStops, int numStops);
bool peopleWaiting(struct Stop* listOfStops);
bool busEmpty(struct Stop** listOfBuses);
int main () {
    
    char inputStop[128];
    char allStops[26];
    char* token;
    int totalBuses, totalStops = 0;
    struct Bus** listBuses;
    struct Stop** listStops;
    // read in stops and separate from delimiter
    if (fgets(inputStop, sizeof(inputStop), stdin) != NULL) { 
        int j = 0;
        token = strtok(inputStop, ",");
        while (token != NULL && j < 26) {
            allStops[j] = token[0];
            token = strtok(NULL, ",");
            j++;
            totalStops++;
        }
    }
    scanf("%d\n", &totalBuses);
    listBuses = (struct Bus**) malloc(sizeof(struct Bus*) * totalBuses);
    if (listBuses == NULL) {
        printf("Error allocating memory for bus items.\n");
        return -1;
    }

    // BUS INITIALIZATION PHASE.
    // READ IN LINE FOR EACH BUS AND INITIALIZE RELEVANT DATA.
    for (int i = 0; i < totalBuses; ++i) {
        char buffer[128];
        struct Bus* busX = (struct Bus*)malloc(sizeof(struct Bus)); 
        if (busX == NULL) {
            printf("Error allocating memory for bus #%d.\n", i);
            return -1;
        }
        
        fgets(buffer, sizeof(buffer), stdin);
        token = strtok(buffer, "|");

        if (token != NULL) {    
            // Bus0|1,2|A|ABC
            // Bus1|1,2|B|AB
            // busName|economySeatsStr,businessSeatsStr|startPoint|validStops
            busX->busName = strdup(token); // busName first token

            token = strtok(NULL, ",");
            busX->numBusinessSeats = atoi(token);

            token = strtok(NULL, "|");
            busX->numEconomySeats = atoi(token);

            token = strtok(NULL, "|");
            busX->currStop = strdup(token);
            token = strtok(NULL, "|");
            busX->validStops = strdup(token);

        }   
        // initialize Heaps (PQueue)
        //busX->ridingEconomy = initializeHeap(NULL, busX->numEconomySeats, 0);
        //busX->ridingBusiness = initializeHeap(NULL, busX->numBusinessSeats, 0);

        listBuses[i] = busX;

        //busX->currStop = busX->validStops[0];
        //busX->indexCurrStop = 0;
        printf("Bus Name: %s\n", busX->busName);
        printf("# Business Seats: %d\n", busX->numBusinessSeats);
        printf("# Economy Seats: %d\n", busX->numEconomySeats);
        printf("Start Point: %s\n", busX->currStop);
        printf("Valid Stops: %s\n", busX->validStops);
       
    } // BUS INITIALIZATION PHASE OVER
    printf("Finished initialization for all buses and their respective data.\n");



    
    // BEGIN STOP AND HEAP INITIALIZATION
    // read in file for each stop (use allStops) (A, B, C)
    
    FILE* stopPtr;
    char stopFileName[64];

    // Create list of Stops to store for ease of freeing memory / debugging
    listStops = (struct Stop**) malloc(sizeof(struct Stop*) * totalStops);
    if (listStops == NULL) {
        printf("Error allocating memory for stop items.\n");
        return -1;
    }
    
    for (int i = 0; i < totalStops; i++) { 
        // open a file for each stop
        // make struct Stop for each stop in allStops[i]

        sprintf(stopFileName, "stop%c.in", allStops[i]);
        stopPtr = fopen(stopFileName, "r");
        if (!stopPtr) {
            printf("Error opening file %s.", stopFileName);
            return -1;
        }
        
        // Create a stop object for each possible stop
        struct Stop* stopX = (struct Stop*)malloc(sizeof(struct Stop));
        if (stopX == NULL) {
            printf("Error allocating memory for stop item.\n");
            return -1;
        }

        // begin to read in line by line, initializing passengers
        
        // Print what stop we're at
        printf("Stop %c Initialization:\n\n", allStops[i]);
        stopX->stopName = strdup(&allStops[i]);

        char buffer[128];
        int stopPassengerCount = 0;

        // Initialize Heaps for stopX
        stopX->businessClass = initializeHeap(NULL, 1, 0);
        stopX->economyClass = initializeHeap(NULL, 1, 0);
        while (fgets(buffer, sizeof(buffer), stopPtr) != NULL) {
            // process buffer (strtok)
            // name|destinations|age|type

            printf("Passenger number %d\n", stopPassengerCount+1);
            printf("--------------------\n");
            token = strtok(buffer, "|");
            if (token != NULL) {
                Person* passenger = (Person*)malloc(sizeof(Person));

                passenger->name = strdup(token);
                printf("Passenger name: %s\n", passenger->name);
                // for destinations, read entire string with possibility of ","
                // check for "," in token using strchr, if it is null then just read in the 
                // one destination, else we know there is multiple destinations
                token = strtok(NULL, "|");
                passenger->destinations = strdup(token);

                char* comma = strchr(passenger->destinations, ',');
                while (comma != NULL) {
                    memmove(comma, comma + 1, strlen(comma));  // Shift the string one position left
                    comma = strchr(comma, ',');  // Move to the next comma
                }
                printf("Passenger destinations allocated: %s\n", passenger->destinations);

                token = strtok(NULL, "|");
                passenger->age = atoi(token);
                printf("Passenger age allocated: %d\n", passenger->age);
                // parse age til |

                token = strtok(NULL, "|");
                passenger->type = strdup(token);      
                printf("Passenger type allocated: %s\n",passenger->type);         
                // parse type

                ++stopPassengerCount;
                
                // By this point, passenger x is fully allocated and initialized. Put into respective PQueue in their Stop
                size_t newlinePos = strcspn(passenger->type, "\n");
                passenger->type[newlinePos] = '\0';

                if (strcmp(passenger->type, "business") == 0) {
                    insert(stopX->businessClass, passenger);
                } else if (strcmp(passenger->type, "economy") == 0) {
                    insert(stopX->economyClass, passenger);
                } else {
                    printf("Passenger is not apart of Economy or Business type. Invalid.\n");
                }
                
                // Handle stop data
                // if passenger type business, insert business, else if economy, insert economy

                // print stopX name
                // print stopX business class size
                // print stopX economy class size
            }
            listStops[i] = stopX; // Put copy into listStops for memory/debug purposes

            //printf("%s\n", stopX->stopName);
            //printHeap(stopX->economyClass);
            //printf("Business class size: %d\n", stopX->businessClass->size);
            //printf("Economy class size: %d\n", stopX->economyClass->size);
            printf("\n");
            if (feof(stopPtr) == EOF) break;

        }
        
    } // FINISHED INITIALIZATION FOR HEAP AND STOP
    // MEMORY LEAKS NEED TO BE FIXED HERE


    printf("Finished initialization for all stops and their respective passengers.\n");

    int timestamp = 0;
   
    // while people are still in buses or at stops
        // for each bus
        //      unload business
        //      unload economy
        //      load business from stop
        //      load economy from stop
        //      increment bus stop
    //  



    //printf("All initialization finished. Beginning simulation.\n");
    for (int i = 0; i < totalBuses; ++i) {
    char fileName[256];
        if (listBuses[i] && listBuses[i]->busName) { // Check if pointers are valid
            sprintf(fileName, "%s.out", listBuses[i]->busName); // Use sprintf instead of fprintf
            
            FILE *file = fopen(fileName, "w"); 
            if (!file) {
                printf("Error opening file %s.\n", fileName);
                return;
            }
            //printf("Output elements for busX file here.\n");
            // Perform file operations here
            fclose(file); // Close the file stream
        } else {
            printf("Invalid bus entry at index %d.\n", i);
            return;
        }
}

    //initiateTransportation(listBuses, totalBuses, listStops, totalStops);
    
    // free memory for allocated structures (listBuses, Stops, bus data)
    freeBuses(listBuses, totalBuses);
    printf("Memory allocated for all bus data has been free'd successfully.\n");
    freeStops(listStops, totalStops);
    printf("Memory allocated for all stop data has been free'd successfully.\n");


    return 0;
}

// Free memory allocated to buses and their data fields.
void freeBuses(struct Bus** listOfBuses, int numBuses) {
    if (listOfBuses == NULL || numBuses == 0) return;

    for (int i = 0; i < numBuses; ++i) {
        free(listOfBuses[i]->busName);
        free(listOfBuses[i]->currStop);
        free(listOfBuses[i]->validStops);
        free(listOfBuses[i]);
    }
    free(listOfBuses);
    listOfBuses = NULL;
    return;
}

// Free memory allocated to stops and their data fields.
void freeStops(struct Stop** listOfStops, int numStops) {
    if (listOfStops == NULL | numStops == 0) return;

    for (int i = 0; i < numStops; ++i) {
        destructHeap(&(listOfStops[i]->businessClass));
        destructHeap(&(listOfStops[i]->economyClass));
        free(listOfStops[i]->stopName);
        free(listOfStops[i]);
    }
    free(listOfStops);
    listOfStops = NULL;
    return;
}

// Finds the index of a given stop from the array holding stop names.
int findStopIndex(char stop, char arrStops[], int totalStops) {
    for (int i = 0; i < totalStops; i++) {
        if (arrStops[i] == stop) return i;
    }
    return -1; // Stop not found
}

// Advances given bus to its next stop
void moveToNextStop(struct Bus* bus) {
   if (bus == NULL || bus->validStops == NULL) {
        return; 
    }

    // Find the current stop in the validStops string
    char* currentStopPtr = strchr(bus->validStops, bus->currStop);
    if (currentStopPtr == NULL) {
        return; // Current stop is not on the route 
    }

    // Calculate the index of the current stop and the index for the next stop
    int currentStopIndex = (int)(currentStopPtr - bus->validStops);
    int nextStopIndex = (currentStopIndex + 1) % strlen(bus->validStops);

    // Update to the next stop
    bus->currStop = bus->validStops[nextStopIndex];
    return;
}

// Maintains destination string after unloading a passenger
void updateDestinations(Person *person) {
    if (person == NULL || person->destinations == NULL) {
        return;
    }

    // Find the comma indicating the end of the current destination.
    char *comma = strchr(person->destinations, ',');
    if (comma != NULL) {
        // Create a new string starting from the character after the comma.
        char *newDestinations = strdup(comma + 1);
        if (newDestinations == NULL) {
            printf("Error duplicating new destinations string.\n");
            return;
        }

        // Free the old destinations string and update the person's destinations.
        free(person->destinations);
        person->destinations = newDestinations; // strdup needed?
    } else {
        return;
    }
}

// Helper function to unload passengers from the bus.
void unloadPassengers(struct Bus* bus, char currentStop) {
    if (bus == NULL) {
        printf("Error: Bus is NULL\n");
        return;
    }
    Heap* tempStopQueue;

    // Whenever unloading a passenger, we need to update their destination if they're being
    // unloaded

    // So, grab the person to be unloaded, check if their first dest is the curr stop
    int holderBusinessSeats = bus->numBusinessSeats;
    for (int i = 0; i < holderBusinessSeats; ++i) {
        Person* tempMax = getMax(bus->ridingBusiness);
        if (tempMax->destinations[0] == currentStop) {
            updateDestinations(tempMax);
            
        }
    }
    // free pointer for extractMax
    // Unload business class passengers
    // for each passenger in bus->ridingBusiness
    //     if passenger's next destination == currentStop
    //         remove passenger from bus->ridingBusiness
    // Repeat for economy class with bus->ridingEconomy
}

// Function to load passengers onto the bus from a stop
void loadPassengers(struct Bus* bus, struct Stop* stop) {
    // Whenever loading a passenger, we need to take out current stop to maintain destinations
    
    // Load business class passengers
    while (bus->ridingBusiness->size > 0 && stop->businessClass->size > 0) {
        Person* maxPerson = getMax(stop->businessClass);
        if (maxPerson && strchr(bus->validStops, maxPerson->destinations[0])) { 
            // Passenger's next destination is on the bus's route
            extractMax(stop->businessClass); // Remove from stop priority queue
            insert(bus->ridingBusiness, maxPerson);
            // Add to bus's passenger list
            // ...
        } else {
            // Passenger's next destination is not on the bus's route
            // This person should be ignored, wait til bus to pick them up
            // ...
            break; 
        }
    }

    // Repeat the same logic for economy class passengers
    while (bus->ridingEconomy->size > 0 && stop->economyClass->size > 0) {
        Person* maxPerson = getMax(stop->economyClass);
        
    }
    return;

}

// Function to write the current state of the bus to a file
void writeBusStateToFile(struct Bus* bus, int timestamp) {
    char filename[256];
    sprintf(filename, "%s.out", bus->busName); 
    
    FILE *file = fopen(filename, "w"); // appending to already opened file
    if (!file) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    fprintf(file, "%d|", timestamp);
    printf("Hello\n");
    // Write business class passengers
    for (int i = 1; i < bus->ridingBusiness->size; ++i) {
        fprintf(file, "%s(business)", bus->ridingBusiness->arr[i].name);
        if (i < bus->ridingBusiness->size || bus->ridingEconomy->size > 0) {
            fprintf(file, " "); // Separator for next passenger
        }
    }
    // Write economy class passengers
    for (int i = 1; i <= bus->ridingEconomy->size; ++i) {
        fprintf(file, "%s(economy)", bus->ridingEconomy->arr[i].name);
        if (i < bus->ridingEconomy->size) {
            fprintf(file, " "); // Separator for next passenger
        }
    }

    if (bus->ridingBusiness->size == 0 && bus->ridingEconomy->size == 0) {
        fprintf(file, "Empty");
    }

    fprintf(file, "\n");
    fclose(file);
}

void initiateTransportation(struct Bus** buses, int totalBuses, struct Stop** stops, int totalStops) {
    int timestamp = 0;
    bool transporting = true;

    // Initialize bus output files to empty
    for (int i = 0; i < totalBuses; i++) {
        char fileName[256];
        sprintf(fileName, "%s.out", buses[i]->busName);
        FILE *file = fopen(fileName, "w"); 
        if (file == NULL) {
            printf("Error opening file %s for initialization.\n", fileName);
            continue;
        }
        fclose(file);
    }

    while (transporting) {
        transporting = false; // just assuming transportation will be false, unless we see it needs to keep going.

        // Process each bus
        for (int i = 0; i < totalBuses; i++) {
            struct Bus* bus = buses[i];

            // If the current stop is valid, unload and load passengers
            if (strchr(bus->validStops, bus->currStop)) {
                unloadPassengers(bus, bus->currStop);
                loadPassengers(bus, stops[findStopIndex(bus->currStop, stops, totalStops)]);
                transporting = true; 
            }

            // Move to the next stop
            moveToNextStop(bus);

            // Write the state of the bus to its file
            writeBusStateToFile(bus, timestamp);
        }

        // if all buses are empty and no stops have waiting passengers
        bool busesEmpty = true;
        for (int i = 0; i < totalBuses; i++) {
            if (buses[i]->ridingBusiness->size > 0 || buses[i]->ridingEconomy->size > 0) {
                busesEmpty = false;
                break;
            }
        }
        bool stopsEmpty = true;
        for (int i = 0; i < totalStops; i++) {
            if (stops[i]->businessClass->size > 0 || stops[i]->economyClass->size > 0) {
                stopsEmpty = false;
                break;
            }
        }

        if (busesEmpty && stopsEmpty) {
            transporting = false; // All passengers have been serviced
        }

        ++timestamp; 
    }
}
