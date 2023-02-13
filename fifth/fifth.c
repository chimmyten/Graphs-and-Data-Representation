#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

struct Node{
    char data[80];
    int edgeWeight;
    struct Node* next;
    int inDeg;
};

struct Stack{
    struct Node* top;
    struct Node* tPrev;
};

struct Stack* createStack(int size){
    struct Stack* stack = malloc(sizeof(struct Stack));
    //stack->top = malloc(size * sizeof(struct Node*));
    stack->top = NULL;
    return stack;
}

struct Node* createVert(char* str, int weight){//creates vertex containing specified string
    struct Node* node = malloc(sizeof(struct Node));
    strcpy(node->data, str);
    node->edgeWeight = weight;
    node->next = NULL;
    node->inDeg = 0;
    return node;
}

void push(struct Stack** stack, char* toPush, int weight){
    

    if ((*stack)->top == NULL){
    
    (*stack)->top = createVert(toPush, weight);
    ((*stack)->top->next) = NULL;
}
    else {
        struct Node* temp = createVert(toPush, weight);
        temp->next = (*stack)->top;
        (*stack)->top = temp;
    }
}

struct Node* pop(struct Stack** stack){
    struct Node* toPop = (*stack)->top;
    ((*stack)->top) = ((*stack)->top->next);
    return toPop;
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

struct Node* reverseList(struct Node** head, struct Node** sourceVertex){
    struct Node* prev = NULL;
    struct Node* current = (*head);
    while (current != NULL){
        struct Node* oldNext = current->next;
        current->next = prev;
        prev = current;
        current = oldNext;
    }
    (*head) = prev;
    (*sourceVertex)->next = (*head);
    return (*head);
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

void topSort(struct Node* source, struct Node** vertArray, int numVertices, struct Node** visited, int* popCounter, struct Node** topSorted){
    struct Stack* dfsStack = createStack(numVertices);
    if (source->inDeg == 0){ //only push if indegree is 0
    push(&dfsStack, source->data, source->edgeWeight);//push source
    } else {
        free(dfsStack);
        return;
    }
   while (dfsStack->top != NULL){//while stack is not empty
        struct Node* popped = pop(&dfsStack);//pop 
        if (visited[indexOf(vertArray, popped->data, numVertices)] != NULL){
            //printf("%s\n", vertArray[indexOf(vertArray, popped->data, numVertices)]->data);
            //printf("%d\n", *popCounter);
            topSorted[*popCounter] = vertArray[indexOf(vertArray, popped->data, numVertices)];
            
            visited[indexOf(vertArray, popped->data, numVertices)] = NULL;//mark node as visisted
            (*popCounter)++;
        }
        struct Node* degRed = vertArray[indexOf(vertArray, popped->data, numVertices)]->next;
        while (degRed != NULL){
            (vertArray[indexOf(vertArray, degRed->data, numVertices)]->inDeg)--;
            degRed = degRed->next;
        }
        struct Node* current = reverseList(&vertArray[indexOf(vertArray, popped->data, numVertices)]->next, &vertArray[indexOf(vertArray, popped->data, numVertices)]); //pointer to adjacency list
        free(popped);
        while(current != NULL){//push all adjacent nodes that have not been visited in reverse order

            if (visited[indexOf(vertArray, current->data, numVertices)] != NULL && (vertArray[indexOf(vertArray, current->data, numVertices)]->inDeg) == 0){
                push(&dfsStack, current->data, current->edgeWeight);
            }
           // printf("%s\n", current->data);
            current = current->next;
        }
    }

     
    

    free(dfsStack);
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

void ssp(char* source, struct Node** vertSorted, int numVertices, struct Node** adjList){
    //set all distances to infinity
    int dist[numVertices];
        for (int i = 0; i < numVertices; i++){
            dist[i] = INT_MAX;
        }
    //set distance to source node to 0
    dist[indexOf(vertSorted, source, numVertices)] = 0;
    for (int i = 0; i < numVertices; i++){
        struct Node* current = adjList[indexOf(adjList, vertSorted[i]->data, numVertices)]->next;
        while (current != NULL){
            if (dist[indexOf(vertSorted, current->data, numVertices)] > dist[indexOf(vertSorted, vertSorted[i]->data, numVertices)] + current->edgeWeight && dist[indexOf(vertSorted, vertSorted[i]->data, numVertices)] < INT_MAX - current->edgeWeight){
                dist[indexOf(vertSorted, current->data, numVertices)] = dist[indexOf(vertSorted, vertSorted[i]->data, numVertices)] + current->edgeWeight;
            }
            current = current->next;
        }
    }
    for (int i = 0; i < numVertices; i++){
    printf("%s ", adjList[i]->data);
    if ( dist[indexOf(vertSorted, adjList[i]->data, numVertices)] == INT_MAX){
        printf("INF\n");
    }
    else{
        printf("%d\n", dist[indexOf(vertSorted, adjList[i]->data, numVertices)]);
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
    
    for(int i = 0; i < numVertices; i++){
        vertArray[i]->inDeg = inDegree(vertArray, numVertices, vertArray[i]->data);
    }
    //array to check if nodes have been visited
    struct Node* visited[numVertices];  
    for (int i = 0; i < numVertices; i++){
        visited[i] = vertArray[i];
    }
    //store the nodes in topsorted order
    struct Node* topSorted[numVertices];

    int popCounter = 0;
    for (int i = 0; i < numVertices; i++){//run dfs on all nodes that haven't been visited to catch disconnected ones
        if(visited[indexOf(vertArray, vertArray[i]->data, numVertices)] != NULL){
            topSort(vertArray[i], vertArray, numVertices, visited, &popCounter, topSorted);
        }
    }
    
    printf("\n");
    if (popCounter != numVertices){
        printf("CYCLE\n");
    }
    else if (popCounter == numVertices){
    
        char query[80];
        while(fscanf(fp2, "%s\n", query) != EOF){
            ssp(query, topSorted, numVertices, vertArray);
        }
    }

    //printf("%d %d\n", inDegree(vertArray, numVertices, vertArray[1]->data), outDegree(&vertArray[1]));

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