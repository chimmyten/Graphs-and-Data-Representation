#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
    
struct Queue{
    struct Node** array; //array of nodes 
    int front;
    int rear;

};

struct Node{
    char data[80];
    struct Node* next;
};

struct Queue* createQueue(int size){
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->array = malloc(size * sizeof(struct Node));
    (queue)->front = -1;
    (queue)->rear = 0;
    return queue;
}

struct Node* createVert(char* str){//creates vertex containing specified string
    struct Node* node = malloc(sizeof(struct Node));
    strcpy(node->data, str);
    node->next = NULL;
    return node;
}

void enqueue(char* str, struct Queue** queue){
    int front = (*queue)->front;
        if (front == -1){//if nothing has been enqueued yet 
        (*queue)->front = 0;
    }
    
    int rear = (*queue)->rear;
    (*queue)->array[rear] = createVert(str);
    ((*queue)->rear)++;
}

struct Node* dequeue(struct Queue** queue){
     struct Node* dequeued = (*queue)->array[(*queue)->front];
    ((*queue)->front)++;
    return dequeued;
}

int indexOf(struct Node** array, char* str, int numVertices){//returns the index of the array with the requested string
    for (int i = 0; i < numVertices; i++){
        if (strcmp(array[i]->data, str) == 0){
            return i;
        }
    }
    return -1;
}

void bfs(struct Node* source, struct Node** vertArray, int numVertices){
    struct Queue* bfsQueue = createQueue(numVertices);
    for (int i = 0; i < numVertices; i++){
        bfsQueue->array[i] = NULL;
    }
    struct Node* visited[numVertices];  
    for (int i = 0; i < numVertices; i++){
        visited[i] = vertArray[i];
    }
    enqueue(source->data, &bfsQueue);//enqueue source
    visited[indexOf(vertArray, source->data, numVertices)] = NULL;
    //printf("%d %d\n", (*queue)->front, (*queue)->rear);
   while (((bfsQueue)->rear) > ((bfsQueue)->front)){//while queue is not empty
        struct Node* dequeued = dequeue(&bfsQueue);//dequeue 
        struct Node* current = vertArray[indexOf(vertArray, dequeued->data, numVertices)]->next; //pointer to adjacency list
        while(current != NULL){//enqueue all adjacent nodes that have not been visited
            if (visited[indexOf(vertArray, current->data, numVertices)] != NULL){
                enqueue(current->data, &bfsQueue);
                visited[indexOf(vertArray, current->data, numVertices)] = NULL;//mark node as visisted

            }
            current = current->next;
        }
        printf("%s ", dequeued->data); //print out dequeued node string
    }
    printf("\n");
    for (int i = 0; i < numVertices; i++){
        if (bfsQueue->array[i] != NULL){
            free(bfsQueue->array[i]);
        }
    }
    free(bfsQueue->array);
    free(bfsQueue);
}


void insertNode(char* str, struct Node** head){ 
    
    if ((*head) == NULL){
        return;
    }
    struct Node* current = *head;
    
    while(current->next != NULL){//insert somewhere in middle of list 
        if (strcmp(str, current->next->data) < 0){
            struct Node* temp = current->next;
            current->next = createVert(str);
            current->next->next = temp;
            return;
        }
        current = current->next;
    }
    if(current->next == NULL){//insert at end of list
        current->next = createVert(str);
    }
    
    return;
}
int degree(struct Node** head){
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

void printList(struct Node** head){
    struct Node* current = NULL;
    
    if ((*head)->next != NULL){
        current = (*head)->next;
    } 
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
        vertArray[i] = createVert(temp);
    }

    while (fscanf(fp, "%s\n", temp) != EOF){//read in which linked list to insert to, which str to insert and insert at correct location
        char second[80];
        fscanf(fp, "%s", second);
        insertNode(second, &vertArray[indexOf(vertArray, temp, numVertices)]);
        insertNode(temp, &vertArray[indexOf(vertArray, second, numVertices)]);
    }

    //BFS Section
    
   

   
    char bfsVertex[80];

    
    while (fscanf(fp2, "%s\n", bfsVertex) != EOF){
        bfs(vertArray[indexOf(vertArray, bfsVertex, numVertices)], vertArray, numVertices);
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