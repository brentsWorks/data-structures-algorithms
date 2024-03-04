#include "Dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Brent Brison
// ID: 2059355
// Professor Pang
// Dictionary Music Application (Program 3)
// November 18, 2023

#define MAX_STR_SIZE 100
#define DICT_SIZE MAX_STR_SIZE
void printSong(void *data) {
    KVPair *song = (KVPair *)data;
    printf("%s by %s\n", song->key, (char *)song->value);
}

void parseCommand(Dictionary*, Dictionary*, Dictionary*, int);

bool isValidCommand(int);

int main(void) {
    // read from STDIN

    char fileName[64];
    FILE* fptr;

    // read in file , open file,
    scanf("%s\n", fileName);
    fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("Error opening file %s\n", fileName);
        return -1;
    }

    //initialize dict from file
    Dictionary* songDict = dictionary_create(DICT_SIZE, printSong);
    Dictionary* authorDict = dictionary_create(DICT_SIZE, printAuthorFreq);
    Dictionary* wordDict = dictionary_create(DICT_SIZE, printWordFreq);
    char buffer [MAX_STR_SIZE];


    // Create Dict for authors and their song freq
    // Every elem read in & insert:

    // if elem->value not found in authorDict (since value = author, which is key in authorDict)
    //      create a KVPair* authorPair for authorDict with ->key = strdup(elem->value)
    //                                                      ->value = 0;
    //    
    // else if elem->value found in authorDict 
    //      increment KVPair* value

    while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
        // Ed Sheeran, mohabbat bhi jhoothi
        KVPair* elem = (KVPair*)malloc(sizeof(KVPair));
        char* token = strtok(buffer, ","); // gather author str up to ","
        if (token != NULL) {
            elem->value = strdup(token); 
            token = strtok(NULL, "\n"); 
            ++token; // move past leading space
            elem->key = strdup(token); // capture song
             
            // songDict insert complete

            // song is valid here. handle wordDict case
            if (dictionary_insert(songDict, elem)) {
                char* songToken = strtok(token, " "); // Tokenize song. 
                while (songToken != NULL) {
                    KVPair* foundWord = dictionary_find(wordDict, songToken);
                    if (foundWord) { // if current word of song title found in wordDict
                        ++foundWord->value;
                    } else {
                        KVPair* wordPair = (KVPair*)malloc(sizeof(KVPair));
                        wordPair->key = strdup(songToken);
                        wordPair->value = 1;
                        dictionary_insert(wordDict, wordPair);
                    }
                    songToken = strtok(NULL, " ");
                }
            }
            // wordDict insert complete (for current song)

            KVPair* foundAuthor = dictionary_find(authorDict, elem->value);
            if (foundAuthor) {
                ++foundAuthor->value;
            } else { // author not in authorDict
                KVPair* authorPair = (KVPair*)malloc(sizeof(KVPair));
                authorPair->key = strdup(elem->value);
                authorPair->value = 1;
                dictionary_insert(authorDict, authorPair);
            }
        }
    }
    fclose(fptr);
    //dictionary_print(wordDict);
    //dictionary_print(songDict);
    //dictionary_print(authorDict);
    // File reading complete, begin command reading

    int commandInput = 0;
    while (true) {
        // prompt user for entry, read in as char and convert to int in function
        int scanOutput = scanf("%d\n", &commandInput);
        if (scanOutput == 1) { // successfully scanned one integer
            if (isValidCommand(commandInput) == false) {
                // invalid input, try again
                printf("Invalid choice! Please select a valid option.\n");
                continue;
            }
            if (commandInput == 8) {
                // exit program (kick out of loop)
                printf("Goodbye! Freeing all memory allocated and exiting program...\n");
                break;
            }
            parseCommand(songDict, authorDict, wordDict, commandInput);
            // else valid and 1-8, parse command
        } else {
            // attempted to read in more than one int, or invalid read 
            printf("Invalid choice! Please select a valid option.\n");
            continue;
        }

    }
    dictionary_destroy(songDict);
    dictionary_destroy(authorDict);
    dictionary_destroy(wordDict);
    // Above loop continues until reading in the exit command '8'
    // Now we know we are exiting the program so we need to free all allocated memory
    return 0;
}

// Helper function to parse command input from stdin
void parseCommand(Dictionary* D, Dictionary* authors, Dictionary* words, int command) {
    // Assume command is valid action (1-7, not exit) when passed in (check before func call) 
    char song [MAX_STR_SIZE];
    char author [MAX_STR_SIZE];
    char* songToken;
    switch (command) { // pass command into switch
        case 1: // add song
            scanf("%[^\n]\n", song);
            scanf("%[^\n]\n", author);
            KVPair* elem = (KVPair*)malloc(sizeof(KVPair));
            elem->key = strdup(song);
            elem->value = strdup(author);
            dictionary_insert(D, elem);

            // check if elem's author doesn't exist yet
            KVPair* foundAuthor = dictionary_find(authors, elem->value);
            if (foundAuthor) {
                ++foundAuthor->value;
            } else { // author not in authorDict
                KVPair* authorPair = (KVPair*)malloc(sizeof(KVPair));
                authorPair->key = strdup(elem->value);
                authorPair->value = 1;
                dictionary_insert(authors, authorPair);
            }

            songToken = strtok(song, " "); // Tokenize song. 
            while (songToken != NULL) {
                KVPair* foundWord = dictionary_find(words, songToken);
                if (foundWord) { // if current word of song title found in wordDict
                    ++foundWord->value;
                } else {
                    KVPair* wordPair = (KVPair*)malloc(sizeof(KVPair));
                    wordPair->key = strdup(songToken);
                    wordPair->value = 1;
                    dictionary_insert(words, wordPair);
                }
                songToken = strtok(NULL, " ");
            }
            break;
        case 2: // remove song
            scanf("%[^\n]\n", song);
            KVPair* foundDel = dictionary_find(D, song);
            if (foundDel) {
                dictionary_delete(D, song);
                KVPair* decAuthorFreq = dictionary_find(authors, foundDel->value);
                --decAuthorFreq->value; // decrement author's frequency (song removed)

                // we know song in dict, lower frequency of each word it contained
                songToken = strtok(song, " "); // Tokenize song. 
                while (songToken != NULL) {
                    KVPair* foundWord = dictionary_find(words, songToken);
                    --foundWord->value;
                    songToken = strtok(NULL, " ");
                }
            } 
           
            break;
        case 3: // update song details
            scanf("%[^\n]\n", song);
            KVPair* toUpdate = dictionary_find(D, song);
            if (!toUpdate) {
                printf("Song to update details for not found.\n");
                break;
            }
            scanf("%[^\n]\n", author);
            KVPair* authorToUpdate = dictionary_find(authors, toUpdate->value);
            free(toUpdate->value); // free value, strdup it
            toUpdate->value = strdup(author);
    
            // decrease original author's frequency (value)
            --authorToUpdate->value;
            // allocate new node and increment value
            KVPair* updatedAuthor = (KVPair*)malloc(sizeof(KVPair));
            updatedAuthor->key = strdup(toUpdate->value);
            updatedAuthor->value = 1;
            dictionary_insert(authors, updatedAuthor);
            break;
        case 4: // find author of song
            scanf("%[^\n]\n", song);
            KVPair* target = dictionary_find(D, song);
            if (target) { // if KVPair found (Not NULL)
                printf("Found song: %s by %s\n", target->key, target->value);
            } else {
                printf("Song not found!\n");
            }
            break;
        case 5: // print all songs and their author
            dictionary_print(D);
            break;
        case 6: // getAuthorWithMostSongs
            getAuthorWithMostSongs(authors);
            break;
        case 7: // getMostPopularWords
            getMostFrequentWords(words);
            break;
        default: // Invalid command, prompt another input and call function again in main
            printf("Invalid choice! Please select a valid option.\n");
            break;
    }
    return;
}

bool isValidCommand(int command) {
    return (command > 0 && command < 9); // valid if between 1 & 8
}

