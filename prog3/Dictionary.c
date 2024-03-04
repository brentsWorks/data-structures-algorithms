#include "List.h"
#include "HashTable.h"
#include "Dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary {
    int slots;
    int size;
    ListPtr *hash_table;
} Dictionary;

// Helper function to implement strdup
// *strdup from <string.h> is not included in c99
// Cite: 
// https://stackoverflow.com/questions/252782/strdup-what-does-it-do-in-c

char *strdup(const char* src) {
    char* dst = malloc(strlen(src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}

// Helper function to serve as a dataPrinter for a authorDict (KV-author:freq)
// printing authorDict (opposite key value order as song Dict)
void printAuthorFreq(void *data) {
    KVPair *author = (KVPair*)data;
    printf("%s: %d song(s)\n", (char *)author->key, (int)author->value);
}

// Helper function to serve as a dataPrinter for a wordDict (KV-word:freq)
void printWordFreq(void* data) {
    KVPair* word = (KVPair*)data;
    printf("%s: %d occurrence(s)\n", (char*)word->key, (int)word->value);
}

// Helper function to find a node with a specific key in a list and return its index.
// Returns -1 if the key is not found.
static int find_key_index(ListPtr L, char *key) {
    for (int i = 0; i < lengthList(L); i++) {
        KVPair *pair = (KVPair *)getList(L, i);
        if (strcmp(pair->key, key) == 0) {
            return i;
        }
    }
    return -1;
}

//Finds and prints each KV pair in given author dictionary with value numSongs (line by line) "Author: # song(s)"
void matchAndPrintMaxAuthorFreqs(Dictionary *D, int numSongs) {
    if (!D) return;
    for (int i = 0; i < D->slots; ++i) { // assuming dictionary with no collisions
        NodePtr tmp = D->hash_table[i]->head;
        if (tmp != NULL) {
            KVPair* tmpData = tmp->data;
            if (tmpData->value == numSongs) { // if this author has # songs = numSongs, print
                D->hash_table[i]->dataPrinter(tmpData);
            }
        }
    }
    return;
    // for each element in dict hash table, call dataPrinter (printSong)
}

void matchAndPrintMaxWordFreqs(Dictionary *D, int numWords) {
    if (!D) return;
    for (int i = 0; i < D->slots; ++i) {
        NodePtr tmp = D->hash_table[i]->head;
        while (tmp != NULL) {
            KVPair* tmpData = tmp->data;
            if (tmpData->value == numWords) {
                D->hash_table[i]->dataPrinter(tmpData);
            }
            // if word freq is not equal to word freq, check next possible word
            tmp = tmp->next;
        }
    }
    return;
}
Dictionary *dictionary_create(int hash_table_size, void (*dataPrinter)(void *data)) {
    // malloc for size Dictionary 
    Dictionary* newDict = (Dictionary*)malloc(sizeof(Dictionary));
    if (newDict == NULL) {
        printf("Memory allocation for new Dictionary failed.\n");
    }

    newDict->hash_table = (ListPtr*)malloc(sizeof(ListPtr) * hash_table_size);
    if (newDict->hash_table == NULL) {
        printf("Memory allocation for Dictionary's hash table failed.\n");
    }
    newDict->slots = hash_table_size;
    newDict->size = 0;
    for (int i = 0; i < hash_table_size; ++i) {
        newDict->hash_table[i] = createList(dataPrinter);
    }
    // size is # elements in dict (0)
    // slots is actual size of array (hash_table_size)

    // Initialize hash_table, malloc for size ListPtr 
    // hash_table[i] dataPrinter = dataPrinter

    return newDict;
    
}

void dictionary_destroy(Dictionary *d) {
    if (!d) return; 

    // if dictionary not null, iterate through each linked list, free all elements

    // if key is dynamically allocated, free key
    // free each KVPair in each linked list
    // free each linked list
    // free dictionary
    for (int i = 0; i < d->slots; ++i) {
        ListPtr currList = d->hash_table[i];
        destroyList(&currList);
    }  
    free(d->hash_table);
    free(d);
    return;
}

bool dictionary_insert(Dictionary *D, KVPair *elem) {
    if (!D || !elem) return false; // if D or elem NULL
    // otherwise, hash element key
    unsigned int index = ht_hash(elem->key, D->slots);
    // check if key is already in dictionary (dictionary_find)
    // if (dictionary_find != NULL), then key exists already (dupe)
    // return false
    
    if (dictionary_find(D, elem->key)) {
        return false;
    }

    // else
    // chain list append KVPair at D->hash_table[h(elem.key)]
    
    if (appendList(D->hash_table[index], elem)) {
        ++D->size;
        return true;
    }
    return false;
    // return true on successful append, otherwise false
   
    
}

KVPair *dictionary_delete(Dictionary *D, char *key) {
    if (!D || !key) return NULL; // Dict or Key is null
    // otherwise, dictionary_find(key)
    KVPair* tmp = dictionary_find(D, key);
    
    if (!tmp) { // if not found
        return NULL;
    }

    // KVPair in hash table, so hash key and find index
    unsigned int hashIndex = ht_hash(key, D->slots);

    // find index of matching key in LL (using LL @ [hashIndex])
    int KV_Index = 0;
    NodePtr traversalPtr = D->hash_table[hashIndex]->head;
    while (traversalPtr != NULL) { // traverse correct list until key matches or NULL
        KVPair* traversalData = traversalPtr->data;
        if (strcmp(traversalData->key, key) == 0) { // if matching key, delete at KV_Index, decrement D->size, return tmp
            deleteList(D->hash_table[hashIndex], KV_Index);
            --D->size;
            return tmp;
        }
        ++KV_Index;
        traversalPtr = traversalPtr->next;
    }
    
     // return NULL if unsuccessful delete
    return tmp;
}

KVPair *dictionary_find(Dictionary *D, char *k) {
    // hash key, get KV pair at that index with matching key
    if (!D || !k) return NULL; // Dict or Key is null
    
    unsigned int index = ht_hash(k, D->slots);
    // go to index, traverse linked list until key matches
    NodePtr traversalPtr = D->hash_table[index]->head;
    KVPair* traversalData;
    while (traversalPtr != NULL) {
        traversalData = traversalPtr->data;
        if (strcmp(traversalData->key, k) == 0) {
            return traversalData;
        }
        traversalPtr = traversalPtr->next;
    }
    return NULL; 
}

//Prints the dictionary to stdout in format k1:v1 k2:v2 ... kn:vn \n
void dictionary_print(Dictionary *D) {
    if (!D) return;

    for (int i = 0; i < D->slots; ++i) {
        ListPtr tmpList = D->hash_table[i];
        printList(tmpList);  
    }
    return;
    // for each element in dict hash table, call dataPrinter (printSong)
}

// do i need to dynamically allocate with strdup when copying a key to authorPair
void getAuthorWithMostSongs(Dictionary *authorDict) {
    if (!authorDict) return;
    
    // We have a valid authorDict (author:frequency)
    // Just have to traverse it, find maxFreq, print any duplicates, if none then print all
    
    // Even though authorDict's hash table is an array of LL,
    // we've maintained it to only contain unique keys, so we only have to check the head
    int maxFreq = 0, maxFreqIndex = 0, duplicateFreq = 0;
    for (int i = 0; i < authorDict->slots; ++i) {
        NodePtr curr = authorDict->hash_table[i]->head;
        KVPair* currAuthorData;
        if (curr != NULL) {  // ensure curr is not null so we access valid data
            currAuthorData = curr->data;
            if (currAuthorData->value > maxFreq) { // compare frequency values
                // new maxFreq, so duplicateFreq = 0
                maxFreq = currAuthorData->value;
                maxFreqIndex = i;
                duplicateFreq = 0;
            } else if ( (currAuthorData->value == maxFreq) && currAuthorData->value != 0) {
                // if matching frequency and curr value isn't 0, we know that
                // there are authors with matching frequencys thus far (besides null authors which DNE),
                ++duplicateFreq;
            }
        }
        // regardless of curr being null or not, check iteration
        
        if (i == authorDict->slots-1) { // finishing last iteration so check if duplicateFreq > 0
            if (duplicateFreq > 0) { // if yes, then print all ties
                matchAndPrintMaxAuthorFreqs(authorDict, maxFreq);
                return;
            }
            else { // no duplicates at last iteration, print maxFreq author: AUTHOR_NAME: maxFreq song(s)
                curr = authorDict->hash_table[maxFreqIndex]->head;
                currAuthorData = curr->data;
                printAuthorFreq(currAuthorData); // print author with max freq
                return;
            }
        } 
    }
    return;
}

void getMostFrequentWords(Dictionary *wordDict) { // printing done in stdout
    if (!wordDict) return;
    
    // iterate to each LL of hashtable in wordDict
    // for each LL, traverse til NULL, keeping track of relative data 
    // (maxFreq, maxFreqIndex, dupFreq)
    
    // make sure head is not null of given LL
    // traverse LL, comparing each nodes data->Value to max freq
    // new maxfreq if needed (if so, reset duplicateFreq, and set maxFreqIndex to i, not j)

    // else if no new maxFreq, check if the current node data value is equal to maxFreq, and
    // that  the curr node  data value isn't 0. if true, then this is a duplicate maxFreq word

    // check last iteration if there is duplicates

    int maxFreq = 0, maxFreqLLIndex = 0, duplicateFreq = 0, mostFreqNodeIndex = 0;
    for (int i = 0; i < wordDict->slots; ++i) {
        NodePtr curr = wordDict->hash_table[i]->head;
        KVPair* currWordData;
        for (int j = 0; j < wordDict->hash_table[i]->length; ++j) {
            if (curr != NULL) {
                currWordData = curr->data;
                if (currWordData->value > maxFreq) {
                    maxFreq = currWordData->value;
                    maxFreqLLIndex = i; // LL index in table
                    mostFreqNodeIndex = j; // node index in LL
                    duplicateFreq = 0;
                } else if ( (currWordData->value == maxFreq) && currWordData->value != 0) {
                    ++duplicateFreq;
                }
            }
            curr = curr->next;
        }

        if (i == wordDict->slots -1) {
            if (duplicateFreq > 0) {
                matchAndPrintMaxWordFreqs(wordDict, maxFreq);
                return;
            } else { // otherwise no duplicates, print max 
                curr = wordDict->hash_table[maxFreqLLIndex]->head;
                for (int location = 0; location < mostFreqNodeIndex && curr != NULL; ++location) {
                    curr = curr->next;
                }
                // at location of node (word) with highest freq
                currWordData = curr->data;
                printWordFreq(currWordData);
                return;
            }
        }
    }
    return;
}