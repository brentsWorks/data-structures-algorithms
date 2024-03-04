#include "BST.h"

// Helper Functions for levelorder Traversal

int height(Node* root) {
    if (root == NULL) return 0;
    
    int leftTreeHeight = height(root->left);
    int rightTreeHeight = height(root->right);

    if (leftTreeHeight > rightTreeHeight) { // Compare longest paths
        return (leftTreeHeight+1);
    } else {
        return (rightTreeHeight+1);
    }
}

void printGivenLevel(Node* root, int level) {
    if (root == NULL) return;

    if (level == 1) { // if at root, print value
        printf("%d ", root->value);
    }
    else if (level > 1) {
        printGivenLevel(root->left, level-1); // print left side of given tree level
        printGivenLevel(root->right, level-1); // print right side of given tree level
    }
    return;
}

// Function to create a new Node
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation for new node failed.\n");
        return NULL;
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to generate BST from a sorted array
Node* sortedArrayToBST(int arr[], int start, int end) {
    if (start > end) 
        return NULL;
    int mid = (start + end)/2;
    Node *root = createNode(arr[mid]);
    root->left = sortedArrayToBST(arr, start, mid - 1);
    root->right = sortedArrayToBST(arr, mid + 1, end);
    return root;
}

// Function to insert a node in BST
Node* insertNode(Node* root, int value) {
    if (root == NULL) return createNode(value);

    if (value < root->value) {
        root->left = insertNode(root->left, value);
    } else {
        root->right = insertNode(root->right, value);
    }
    return root;
}

// Helper function to find the inorder successor
static Node* minValueNode(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Function to delete a node in BST

Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return root; //not found
    }

    // Locate node being deleted
    
    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    } else { // equal to root->value
        if (root->left == NULL) { // no left child
            Node* holder = root->right;
            free(root);
            root = NULL;
            return holder;
        } else if (root->right == NULL) { // no right child
            Node* holder = root->left;
            free(root);
            root = NULL;
            return holder;
        }
        
        Node* holder = minValueNode(root->right); // Inorder successor
        root->value = holder->value; // Root receives Inorder Successor value
        root->right = deleteNode(root->right, holder->value); // Delete the inorder successor once it's copied
        
    }
    return root; // return tree with node deleted

}

// Function for inorder traversal of BST
void inorderTraversal(Node* root) { // left middle right
    if (root == NULL) return;

    inorderTraversal(root->left);
    printf("%d ", root->value);
    inorderTraversal(root->right);

    return;
}

// Function for preorder traversal of BST 
void preorderTraversal(Node* root) { // root left right
    if (root == NULL) return;

    printf("%d ", root->value);
    preorderTraversal(root->left);
    preorderTraversal(root->right);

    return;
}

// Function for postorder traversal of BST
void postorderTraversal(Node* root) { // left right root
    if (root == NULL) return;

    postorderTraversal(root->left);
    postorderTraversal(root->right);
    printf("%d ", root->value);

    return;
}

// Function to find a node in BST
Node* findNode(Node* root, int value) {
    if (root == NULL || root->value == value) return root;

    if (root->value > value) {
        return findNode(root->left, value);
    } else {
        return findNode(root->right, value);
    }
}

// Function to free nodes in BST
void freeTree(Node** root) {
    if (root == NULL || (*root) == NULL) return;

    // else free left subtree, then right subtree, then root
    // post order memory de-allocation

    freeTree(&(*root)->left);
    freeTree(&(*root)->right);
    free((*root));
    return;
}
// Function for levelorder traversal of BST
void levelorderTraversal(Node* root){
    // Find height of tree, traverse level by level until height reached
    if (root == NULL) return;

    int treeHeight = height(root);
    for (int i = 1; i <= treeHeight; ++i) { // start at 1 since level 0 doesnt exist (root is height 1)
        printGivenLevel(root, i);
    }
    return;
}

