
#include "BST.h"
#include <stdbool.h>
#include <stdio.h>

// Brent Brison
// Student ID: 2059355
// CSE 101
// Professor Pang
// 21 November 2023
// Binary Search Tree Implementation

int parseCommand(Node*, int);

int main() {
    
    // read in number of elements in a sorted array
    // read in sorted array itself (stdin)

    // generate sorted array
    int arraySize;
    scanf("%d", &arraySize);

    int array[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        scanf("%d", &array[i]);
    }
    Node* root = sortedArrayToBST(array, 0, arraySize-1);
    printf("\n");
    // Command 1: Insert a Node
    // Command 2: Delete a Node
    // Command 3: Find a Node
    // Command 4: Inorder Traversal
    // Command 5: Preorder Traversal
    // Command 6: Postorder Traversal
    // Command 7: Exit
    int command = 0;
    int retvalue = 0;
    // read in first value (command), continue reading same line if valid command
    // so if command is valid (1-7), switch to that command and read in parameters
    while (retvalue != -1) {
        scanf("%d", &command);
        retvalue = parseCommand(root, command);
    }
    freeTree(&root);
    return 0;
}

int parseCommand(Node* rootTree, int command) {
    int value = 0;
    int ch;
    printf("%d\n", command);
    switch (command) {
        case 1:
            // insertNode(root, value);
            scanf("%d\n", &value);
            insertNode(rootTree, value);
            break;
        case 2:
            // deleteNode(root, value);
            scanf("%d\n", &value);
            deleteNode(rootTree, value);
            break;
        case 3:
            // findNode(root, value);
            scanf("%d\n", &value);
            Node* found = findNode(rootTree, value);
            if (found) {
                printf("Node found: %d\n", found->value);
            }
            break;
        case 4:
             // inorderTraversal(root);
             ch = 0;
             while ((ch = getchar()) != '\n' && ch != EOF);  
             inorderTraversal(rootTree);
             printf("\n");
             break;
        case 5:
             // preorderTraversal(root);
             ch = 0;
             while ((ch = getchar()) != '\n' && ch != EOF);  
             preorderTraversal(rootTree);
             printf("\n");
             break;
        case 6:
            // postorderTraversal(root);
            ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF);  
            postorderTraversal(rootTree);
            printf("\n");
            break;
        case 7:
            printf("Exiting program...\n");
            return -1;
        case 8:
            // levelorderTraversal(root);
            ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF);
            levelorderTraversal(rootTree);
            printf("\n");
            break;
        default:
            //printf("%d\n", command);
            printf("Invalid read. Exiting program.\n");
            return -1;
    }        
    return 0;
}
