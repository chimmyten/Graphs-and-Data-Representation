#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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

void bubSort(struct Node** array, int numVertices){
    char temp[80];
    for (int i = 0; i < numVertices; i++){
        for (int j = 0; j < numVertices - i - 1; j++){
            if (strcmp(array[j]->data, array[j+1]->data) > 0){
                strcpy(temp, array[j]->data);
                strcpy(array[j]->data, array[j+1]->data);
                strcpy(array[j+1]->data, temp);
            }
        }
    }
}

struct Node* pop(struct Node** head){ //pop the head of the list which should have the shortest edge
    struct Node* popped = *head;
    (*head) = (*head)->next;
    return popped;
}

void push(struct Node** head, struct Node* toInsert){
    if (*head == NULL){ //if list is empty
        *head = toInsert;
        return;
    }
    if ((*head)->edgeWeight > toInsert->edgeWeight){ //if it should be inserted before head
        struct Node* temp = (*head);
        (*head) = toInsert;
        (*head)->next = temp;
    }
    else { //insert in the middle or at the end
        struct Node* current = *head;
        while (current->next != NULL){
            if (current->next->edgeWeight > toInsert->edgeWeight){
                struct Node* temp = current->next;
                current->next = toInsert;
                current->next->next = temp;
                return;
            }
            current = current->next;
        }
        if (current->next == NULL){
            current->next = toInsert;
            return;
        }
    }
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

void dijkstra(char* source, struct Node** adjlist, int numVertices){
    struct Node* head = NULL;
    int dist[numVertices]; //create distance array
    for (int i = 0; i < numVertices; i++){ //initialize all distances to 0;
        dist[i] = INT_MAX;
    }
    dist[indexOf(adjlist, source, numVertices)] = 0; //initialize distance to source to 0
    push(&head, createVert(source, adjlist[indexOf(adjlist, source, numVertices)]->edgeWeight)); //push source in priority queue
    while (head != NULL){ //while priority queue isn't empty
        
        struct Node* popped = pop(&head); //pop the minimum distance vertex
        struct Node* current = adjlist[indexOf(adjlist, popped->data, numVertices)]->next; 
        while (current != NULL){ //loop through all adjacent vertices
            if (dist[indexOf(adjlist, current->data, numVertices)] > dist[indexOf(adjlist, popped->data, numVertices)] + current->edgeWeight){ //if the distance to the adjacent vertex is greater than the new path found
                dist[indexOf(adjlist, current->data, numVertices)] = dist[indexOf(adjlist, popped->data, numVertices)] + current->edgeWeight; //the new shortest distance is the new path found
                push(&head, createVert(current->data, adjlist[indexOf(adjlist, current->data, numVertices)]->edgeWeight)); //push the adjacent vertex into the pq
            }
            current = current->next;
        }
        free(popped);
    }
    for (int i = 0; i < numVertices; i++){
        printf("%s ", adjlist[i]->data);
        if (dist[indexOf(adjlist, adjlist[i]->data, numVertices)] == INT_MAX){
            printf("INF\n");
        }
        else{
            printf("%d\n", dist[indexOf(adjlist, adjlist[i]->data, numVertices)]);
        }
    }
    printf("\n");
    
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

    bubSort(vertArray, numVertices);

    while (fscanf(fp, "%s\n", temp) != EOF){//read in which linked list to insert to, which str to insert and insert at correct location
        char second[80];
        int readWeight;
        fscanf(fp, "%s", second);
        fscanf(fp, "%d", &readWeight);
        insertNode(second, &vertArray[indexOf(vertArray, temp, numVertices)], readWeight);
        
    }
    
    char query[80];
    while (fscanf(fp2, "%s\n", query) != EOF){
        dijkstra(query, vertArray, numVertices);
    }

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