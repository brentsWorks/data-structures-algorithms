#include "BST.h"
#include <stdlib.h>

int main () {
    // test insert

    // test delete
    
    int array[] = {1, 2, 3, 4, 5, 6};
    int arraySize = (sizeof(array)/sizeof(array[0]));
    printf("%d\n", arraySize);

    Node* bst = sortedArrayToBST(array, 0, arraySize);
    inorderTraversal(bst);
}