#include "HashTable.h"

unsigned long ht_string2int(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//TODO: Implement
unsigned int ht_hash(char *key, unsigned int slots) {
    // convert key to int
    unsigned long keyConv = ht_string2int(key);

    // Division Method
    // k mod m
	return (keyConv % slots);
}
