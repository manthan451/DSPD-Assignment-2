#include <stdio.h>
#include <stdlib.h>
#define SIZE 3

typedef struct DataNode
{
    int data[SIZE];
    int size;
    struct DataNode *prev;
    struct DataNode *next;
    struct KeyNode *parent;
}DataNode;


typedef struct KeyNode
{
    int KeyVal[SIZE];
    int size;
    int isKeyptr;
    union Pointer
    {
        struct KeyNode *Keyptr[SIZE+1];
        DataNode *Dataptr[SIZE];
    }ptr;
}KeyNode;

KeyNode *CreateKey(int val, int isKey)
{
    KeyNode *kptr = (KeyNode *)malloc(sizeof(KeyNode));
    (kptr->KeyVal)[0] = val;
    kptr->size = 1;
    if(isKey == 1)
    {
        kptr->isKeyptr = 1;
        for(int i = 0; i < SIZE+1; i++)
        {
            (kptr->ptr).Keyptr[i] = NULL;
        }
    }
    else
    {
        kptr->isKeyptr = 0;
        for(int i = 0; i < SIZE + 1; i++)
        {
            (kptr->ptr).Dataptr[i] = NULL;
        }
    }
    return kptr;
}

DataNode *CreateDataNode(int data, KeyNode *par)
{
    DataNode *dptr = (DataNode *)malloc(sizeof(DataNode));
    dptr->data[0] = data;
    dptr->size = 1;
    dptr->next = dptr->prev = NULL;
    dptr->parent = par;
    return dptr;
}



void main()
{
    KeyNode *newKey = CreateKey(2,1);
    printf("%d %d", newKey->isKeyptr, newKey->KeyVal[newKey->size - 1]);
    
}
