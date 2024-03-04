#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* removeFileExtension(const char* fileName) {
    char* originalName = strdup(fileName);
    char* dot = strrchr(originalName, '.');
    if (dot) {
        *dot = '\0';  //null terminate to remove extension
    }
    return originalName;
}

void processCommandOut(FILE* output, ListPtr wordList, char justification, int lineWidth) {
    NodePtr currentNode = wordList->head;
    char* word;
    char buffer[lineWidth + 1]; // keep in mind '\0'
    memset(buffer, 0, sizeof(buffer));
    int currentLineLength = 0, count = 0;

    for (int i = 0; i < wordList->length; ++i) {
        if (strcmp(getList(wordList, i), "") == 0) {
            deleteList(wordList, i);
        }
    }

    while (currentNode != NULL) {
        word = (char*)currentNode->data;
        int wordLength = strlen(word);

        if (count + wordLength + currentLineLength <= lineWidth) {
            if (count > 0) {
                strcat(buffer, " ");
                ++currentLineLength;
            }
            strcat(buffer, word);
            ++count;
            currentLineLength += wordLength;
            currentNode = currentNode->next;
        } else {
            if (justification == 'R') {
                fprintf(output, "%*s\n", lineWidth, buffer);
            } else if (justification == 'C') {
                return; // Centered justification hasn't been handled here
            } else { // justification is Left, or default
                fprintf(output, "%s\n", buffer);
            }
            count = 0;
            currentLineLength = 0;
            buffer[0] = '\0';
        }
    }
    
    if (currentLineLength > 0) {
        fprintf(output, "%s\n", buffer);
    }

    return;
}

int main(int argc, char **argv) {
    char fileName[256], outFileName[256], command[256], justification;
    int width;
    FILE *fptr;

    ListPtr L = createList(NULL);

    scanf("%s", fileName);


    fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("Error! Please try entering a valid filename.\n");
        return 1;
    }

    char block[256];
    while (fscanf(fptr, "%s", block) != EOF) {
        appendList(L, strdup(block));
    }
    fclose(fptr);

	char* fixedFileName = removeFileExtension(fileName);
    while(true) {
        FILE* outputFile;

        scanf("%s", command);
        if (strcmp(command, "format") == 0) {
            scanf(" %c%d", &justification, &width);
            sprintf(outFileName, "%s.%c%d", fixedFileName, justification, width);
            outputFile = fopen(outFileName, "w");
            
            if (!outputFile) {
                printf("Error. Unable to open output file\n");
                continue;
            }
            
            processCommandOut(outputFile, L, justification, width);
            fclose(outputFile);
        } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command '%s'. Please use the 'format <Justification> <Width>' format or exit using 'quit'/'exit'.\n", command);
        }
    }

    destroyList(&L);
    return 0;
}