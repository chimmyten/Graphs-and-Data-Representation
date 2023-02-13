#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node{
    char data[80];
    int edgeWeight;
    struct Node* next;
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

void dfs(struct Node* source, struct Node** vertArray, int numVertices, struct Node** visited){
    struct Stack* dfsStack = createStack(numVertices);
    
    
    push(&dfsStack, source->data, source->edgeWeight);//push source

    
   while (dfsStack->top != NULL){//while stack is not empty
        struct Node* popped = pop(&dfsStack);//pop 
        if (visited[indexOf(vertArray, popped->data, numVertices)] != NULL){
            printf("%s ", popped->data); //print out dequeued node string
            visited[indexOf(vertArray, popped->data, numVertices)] = NULL;//mark node as visisted
        }
        struct Node* current = reverseList(&vertArray[indexOf(vertArray, popped->data, numVertices)]->next, &vertArray[indexOf(vertArray, popped->data, numVertices)]); //pointer to adjacency list
        free(popped);
        while(current != NULL){//push all adjacent nodes that have not been visited in reverse order
            if (visited[indexOf(vertArray, current->data, numVertices)] != NULL){
                push(&dfsStack, current->data, current->edgeWeight);
            }
            //printf("%s\n", current->data);
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

int main(int argc, char** argv){

    FILE* fp = fopen(argv[1], "r");
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
    
    


    
    struct Node* visited[numVertices];  
    for (int i = 0; i < numVertices; i++){
        visited[i] = vertArray[i];
    }

    for (int i = 0; i < numVertices; i++){//run dfs on all nodes that haven't been visited to catch disconnected ones
        if(visited[indexOf(vertArray, vertArray[i]->data, numVertices)] != NULL){
            dfs(vertArray[i], vertArray, numVertices, visited);
        }
    }
    printf("\n");




    for(int i = 0; i < numVertices; i++){//freeing each node in each linked list of the array
        struct Node* freePtr = vertArray[i];
        while (freePtr != NULL){
            struct Node* temp = freePtr;
            freePtr = freePtr->next;
            free(temp);
        }
    }

    /*struct Stack* testStack = createStack(numVertices);
    push(&testStack, "1", 10);
    push(&testStack, "2", 10);
    push(&testStack, "3", 10);
    pop(&testStack);
    printf("%s\n", pop(&testStack)->data);*/
    
    //freeing all of the stuck for stack
    



    return 0;
}