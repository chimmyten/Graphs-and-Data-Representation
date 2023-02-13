#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node{
    char data[80];
    int edgeWeight;
    struct Node* next;
};

struct Node* createVert(char* str, int weight){//creates vertex containing specified string
    struct Node* node = malloc(sizeof(struct Node));
    strcpy(node->data, str);
    node->edgeWeight = weight;
    node->next = NULL;
    return node;
}

int indexOf(struct Node** array, char* str, int numVertices){//returns the index of the array with the requested string
    for (int i = 0; i < numVertices; i++){
        if (strcmp(array[i]->data, str) == 0){
            return i;
        }
    }
    return -1;
}

void insertNode(char* str, struct Node** head, int weight){ 
    
    if ((*head) == NULL){
        return;
    }
    struct Node* current = *head;
    
    while(current->next != NULL){//insert somewhere in middle of list 
        if (strcmp(str, current->next->data) < 0){
            struct Node* temp = current->next;
            current->next = createVert(str, weight);
            current->next->next = temp;
            return;
        }
        current = current->next;
    }
    if(current->next == NULL){//insert at end of list
        current->next = createVert(str, weight);
    }
    
    return;
}
int outDegree(struct Node** head){
     struct Node* current = NULL;
    if ((*head)->next != NULL){
        current = (*head)->next;
    }
    else return 0;
    
    int counter = 0;

    while (current != NULL){
        counter++;
        current = current->next;
    }
    return counter;
}

int inDegree(struct Node** array, int numVertices, char* str){
    int inCounter = 0;
    for(int i = 0; i < numVertices; i++){
        struct Node* current = array[i];
        while (current->next != NULL){
            if (strcmp(current->next->data, str) == 0){
                inCounter++;
            }
            current = current->next;
            
        }
    }
    return inCounter;
}

void printList(struct Node** head){
    struct Node* current = NULL;
    
    if ((*head)->next != NULL){
        current = (*head)->next;
    } 
    //printf("%s", current->data);
    while (current != NULL){
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
    return;
}

int main(int argc, char** argv){

    FILE* fp = fopen(argv[1], "r");
    FILE* fp2 = fopen(argv[2], "r");
    int numVertices;
    fscanf(fp, "%d", &numVertices);
    
    struct Node* vertArray[numVertices]; //array holding all of the vertices
    
    char temp[80];

    for (int i = 0; i < numVertices; i++){ //populate array with vertices
        fscanf(fp, " %s\n", temp);
        vertArray[i] = createVert(temp, 0);
    }

    while (fscanf(fp, "%s\n", temp) != EOF){//read in which linked list to insert to, which str to insert and insert at correct location
        char second[80];
        int readWeight;
        fscanf(fp, "%s", second);
        fscanf(fp, "%d", &readWeight);
        insertNode(second, &vertArray[indexOf(vertArray, temp, numVertices)], readWeight);
        
    }

    //printing
    char d_or_a[80];
    char vert[80];
    while (fscanf(fp2, "%s\n",d_or_a) != EOF){
        fscanf(fp2, "%s", vert);
        if (strcmp(d_or_a, "o") == 0){//print out degree of linked list
            printf("%d\n", outDegree(&vertArray[indexOf(vertArray, vert, numVertices)]));
        }
        if (strcmp(d_or_a, "i") == 0){
            printf("%d\n", inDegree(vertArray, numVertices, vert));
        }
        if (strcmp(d_or_a, "a") == 0){//print out all nodes of linked list
            printList(&vertArray[indexOf(vertArray, vert, numVertices)]);
        }
    }
    
    /*for (int i = 0; i < numVertices; i++){//printing for testing
        struct Node* printCurrent = vertArray[i]->next;
        while (printCurrent != NULL){
            printf("%s\n", printCurrent->data);
            printCurrent = printCurrent->next;
        }
    }*/


    for(int i = 0; i < numVertices; i++){//freeing each node in each linked list of the array
        struct Node* freePtr = vertArray[i];
        while (freePtr != NULL){
            struct Node* temp = freePtr;
            freePtr = freePtr->next;
            free(temp);
        }
    }

    return 0;
}