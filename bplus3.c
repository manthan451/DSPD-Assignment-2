#include <stdio.h>
#include <stdlib.h>
#define ORDER 3     //no of children 2/3  no of keys 1/2
#define MAXCAPACITY 3   
#define MINCAPACITY 2

typedef struct date
{
    int day;
    int month;
    int year; // in 2 dig
} date;

typedef struct item_tag
{
    int item_id;
    char *itemname;
    int quantity;
    date *expiry_date;
    int threshold_quantity;
    struct item_tag *prev;
    struct item_tag *next;
} Item;

typedef struct DataNode
{
    Item *ItemArr[MAXCAPACITY + 1];     //MAXCAPACITY + 1 + 1//when it crosses max cap, we split 
    int size;
    int ParentIndex;
    struct DataNode *prev;
    struct DataNode *next;
    struct KeyNode *parent;
}DataNode;

typedef struct KeyNode
{
    int KeyVal[ORDER-1];  //max can be order-1
    int size;
    int utag;   //1 for Keyptr, 2 for Dataptr
    int ParentIndex;
    struct KeyNode *parent;
    union Pointer
    {
        struct KeyNode *Keyptr[ORDER];
        DataNode *Dataptr[ORDER];
    }ptr;

}KeyNode;

KeyNode* InsertintoDataNode(DataNode *dptr, Item *iptr, KeyNode* origin_root);

DataNode* EmptyDataNode()
{
    DataNode* d = (DataNode*)malloc(sizeof(DataNode));
    d->next = d->prev = NULL;
    d->size = 0;
    d->ParentIndex = -1;
    return d;
}

KeyNode* initializeBpTree(int key)      //for order 3, cap 3
{
    KeyNode* root = (KeyNode*)malloc(sizeof(KeyNode));
    root->KeyVal[0] = key;
    root->size = 1;
    root->utag = 2;
    root->parent = NULL;
    root->ParentIndex = -1;

    root->ptr.Dataptr[0] = EmptyDataNode();
    root->ptr.Dataptr[0]->ParentIndex = 0;
    root->ptr.Dataptr[0]->parent = root;
    root->ptr.Dataptr[1] = EmptyDataNode();
    root->ptr.Dataptr[1]->ParentIndex = 1;
    root->ptr.Dataptr[1]->parent = root;

    root->ptr.Dataptr[0]->next = root->ptr.Dataptr[1];
    root->ptr.Dataptr[1]->prev = root->ptr.Dataptr[0];  

    return root;
}

KeyNode* Insert(KeyNode* root, Item *iptr, KeyNode* origin_root)      //for order 3, no of keys 1 or 2
{
    printf("\nInsert function with value %d\n", iptr->item_id);
    int newval = iptr->item_id;
    int i;

    for(i = 0; i < root->size; i++)
    {
        if(newval < root->KeyVal[i])
        {
            if(root->utag == 2)
            {
                printf("going left of %d which is at index %d \n", root->KeyVal[i], i);
                return InsertintoDataNode(root->ptr.Dataptr[i], iptr, origin_root);
            }
            else
            {
                printf("going left of %d\n", root->KeyVal[i]);
                return Insert(root->ptr.Keyptr[i], iptr, origin_root);
            }
        }
    }

    if(root->utag == 2)
    {
        printf("going right of %d\n", root->KeyVal[i-1]);
        return InsertintoDataNode(root->ptr.Dataptr[i], iptr, origin_root);
    }
    else
    {
        printf("going right of %d\n", root->KeyVal[i-1]);
        return Insert(root->ptr.Keyptr[i], iptr, origin_root);  //here i = size, last child
    }
}

KeyNode* PushKeyUp(int key, KeyNode* p,int parentindex, KeyNode* k1, KeyNode* k2, KeyNode* origin_root) //push key into keynode p and also set parent index and parent of k1 k2
{
    printf("Inside push key up\n");
    if(p == NULL)
    {
        KeyNode *neworigin_root = (KeyNode *)malloc(sizeof(KeyNode));
        neworigin_root->size = 1;
        neworigin_root->parent = NULL;
        neworigin_root->KeyVal[0] = key;
        neworigin_root->ParentIndex = -1;
        neworigin_root->utag = 1;

        neworigin_root->ptr.Keyptr[0] = k1;
        neworigin_root->ptr.Keyptr[0]->parent = neworigin_root;
        neworigin_root->ptr.Keyptr[0]->ParentIndex = 0;

        neworigin_root->ptr.Keyptr[1] = k2;
        neworigin_root->ptr.Keyptr[1]->parent = neworigin_root;
        neworigin_root->ptr.Keyptr[1]->ParentIndex = 1;

        printf("New origin root made with key %d\n", key);
        return neworigin_root;
    }
    if(p->size == 1)
    {
        printf("push key case 1, p->size == 1\n");

        if(parentindex == 1)
        {
            p->KeyVal[1] = key;
            p->size = 2;

            p->ptr.Keyptr[1] = k1;
            p->ptr.Keyptr[1]->parent = p;
            p->ptr.Keyptr[1]->ParentIndex = 1;

            p->ptr.Keyptr[2] = k2;
            p->ptr.Keyptr[2]->parent = p;
            p->ptr.Keyptr[2]->ParentIndex = 2;
        }
        else if(parentindex == 0)
        {
            p->KeyVal[1] = p->KeyVal[0];
            p->KeyVal[0] = key;

            p->size = 2;
            p->ptr.Keyptr[2] = p->ptr.Keyptr[1];
            p->ptr.Keyptr[2]->ParentIndex = 2;

            p->ptr.Keyptr[1] = k2;
            p->ptr.Keyptr[1]->ParentIndex = 1;

            p->ptr.Keyptr[0] = k1;
            p->ptr.Keyptr[0]->ParentIndex = 0;
        }

        return origin_root;  
    }
    else if(p->size == 2)
    {
        printf("push key case 2, p->size == 2\n");
        KeyNode *newk1, *newk2;
        int nextpush;
        if(parentindex == 0)
        {
            newk1 = (KeyNode *)malloc(sizeof(KeyNode));
            newk1->KeyVal[0] = key;
            newk1->size = 1;
            newk1->utag = 1;
            newk1->ptr.Keyptr[0] = k1;
            newk1->ptr.Keyptr[0]->ParentIndex = 0;
            newk1->ptr.Keyptr[0]->parent = newk1;
            newk1->ptr.Keyptr[1] = k2;
            newk1->ptr.Keyptr[1]->ParentIndex = 1;
            newk1->ptr.Keyptr[1]->parent = newk1;

            nextpush = p->KeyVal[0];
            p->KeyVal[0] = p->KeyVal[1];
            p->ptr.Keyptr[0] = p->ptr.Keyptr[1];
            p->ptr.Keyptr[1] = p->ptr.Keyptr[2];
            p->size = 1;
            p->ptr.Keyptr[0]->ParentIndex = 0;
            p->ptr.Keyptr[0]->parent = p;
            p->ptr.Keyptr[1]->ParentIndex = 1;
            p->ptr.Keyptr[1]->parent = p;
            newk2 = p;

        }
        else if(parentindex == 1)
        {
            nextpush = key;

            p->size = 1;
            p->ptr.Keyptr[1] = k1;
            p->ptr.Keyptr[1]->ParentIndex = 1;
            p->ptr.Keyptr[1]->parent = p;

            KeyNode *temp = p->ptr.Keyptr[2];
            newk1 = p;

            newk2 = (KeyNode*)malloc(sizeof(KeyNode));
            newk2->KeyVal[0] = p->KeyVal[1];
            newk2->size = 1;
            newk2->utag = 1;
            newk2->ptr.Keyptr[0] = k2;
            newk2->ptr.Keyptr[0]->ParentIndex = 0;
            newk2->ptr.Keyptr[0]->parent = newk2;

            newk2->ptr.Keyptr[1] = temp;
            newk2->ptr.Keyptr[1]->ParentIndex = 1;
            newk2->ptr.Keyptr[1]->parent = newk2;
        }
        else if(parentindex == 2)
        {
            p->size = 1;
            newk1 = p;

            nextpush = p->KeyVal[1];

            newk2 = (KeyNode*)malloc(sizeof(KeyNode));
            newk2->KeyVal[0] = key;
            newk2->size = 1;
            newk2->utag = 1;
            newk2->ptr.Keyptr[0] = k1;
            newk2->ptr.Keyptr[0]->ParentIndex = 0;
            newk2->ptr.Keyptr[0]->parent = newk2;

            newk2->ptr.Keyptr[1] = k2;
            newk2->ptr.Keyptr[1]->ParentIndex = 1;
            newk2->ptr.Keyptr[1]->parent = newk2;
        }

        if(p == origin_root)
        {
            KeyNode *newRoot = (KeyNode*)malloc(sizeof(KeyNode));
            newRoot->KeyVal[0] = nextpush;
            newRoot->size = 1;
            newRoot->utag = 1;
            newRoot->ptr.Keyptr[0] = newk1;
            newRoot->ptr.Keyptr[0]->ParentIndex = 0;
            newRoot->ptr.Keyptr[1] = newk2;
            newRoot->ptr.Keyptr[1]->ParentIndex = 1;

            return newRoot;
        }
        else
        {
            return PushKeyUp(nextpush, p->parent, p->ParentIndex, newk1, newk2, origin_root);
        }
    }
}

KeyNode* InsertintoDataNode(DataNode *dptr, Item *iptr, KeyNode* origin_root)
{
    printf("\nInside InsertintoDataNode with value of newitem = %d\n", iptr->item_id);
    int i = 0;
    Item *temp, *temp2;
    while(i < dptr->size && dptr->ItemArr[i]->item_id < iptr->item_id)
    {
        i++;
    }
    temp = dptr->ItemArr[i];
    dptr->ItemArr[i] = iptr;
    i++;
    while (i < dptr->size)
    {
        temp2 = dptr->ItemArr[i];
        dptr->ItemArr[i] = temp;
        temp = temp2;
        i++;
    }
    if(i <= MAXCAPACITY)
        dptr->ItemArr[i] = temp;

    printf("Current data node after adding becomes\n");
    for(i = 0; i < dptr->size ; i++)
    {
        printf("%d\n", dptr->ItemArr[i]->item_id);
    }

    if (dptr->size < MAXCAPACITY)   //general
    {
        printf("first case of insertion, into same datanode\n");
        dptr->size += 1;
        return origin_root;
    }
    else if(dptr->parent->size < ORDER-1)   //1 //splitting
    {
        printf("second case of insertion, datanode split and new root keyval\n");
        DataNode *d2 = EmptyDataNode();
        d2->ItemArr[0] = dptr->ItemArr[2];
        d2->ItemArr[1] = dptr->ItemArr[3];
        dptr->ItemArr[2] = NULL;
        dptr->ItemArr[3] = NULL;

        d2->size = 2;
        dptr->size = 2;

        d2->next = dptr->next;
        d2->prev = dptr;
        dptr->next = d2;
        if(d2->next)
            d2->next->prev = d2;

        if(dptr->ParentIndex == 1)
        {
            dptr->parent->KeyVal[1] = d2->ItemArr[0]->item_id;
            dptr->parent->ptr.Dataptr[2] = d2;
            dptr->parent->ptr.Dataptr[2]->ParentIndex = 2;
        }
        else if(dptr->ParentIndex == 0)
        {
            dptr->parent->KeyVal[1] = dptr->parent->KeyVal[0];
            dptr->parent->KeyVal[0] = d2->ItemArr[0]->item_id;

            dptr->parent->ptr.Dataptr[2] = dptr->parent->ptr.Dataptr[1];
            dptr->parent->ptr.Dataptr[2]->ParentIndex = 2;
    
            dptr->parent->ptr.Dataptr[1] = d2;
            dptr->parent->ptr.Dataptr[1]->ParentIndex = 1;
        }
        dptr->parent->size ++;
        d2->parent = dptr->parent;
        
        printf("new 2 keyvals = %d and %d\n", dptr->parent->KeyVal[0], dptr->parent->KeyVal[1]);
        return origin_root;
    }
    else    //third case of insertion
    {
        printf("Im in the third case of insertion\n");

        DataNode *d2 = EmptyDataNode();
        d2->ItemArr[0] = dptr->ItemArr[2];
        d2->ItemArr[1] = dptr->ItemArr[3];
        dptr->ItemArr[2] = NULL;
        dptr->ItemArr[3] = NULL;

        d2->size = 2;
        dptr->size = 2;

        d2->next = dptr->next;
        d2->prev = dptr;
        dptr->next = d2;
        if(d2->next)
            d2->next->prev = d2;

        KeyNode* gparent = dptr->parent->parent; //storing the grandparent
        KeyNode* parent = dptr->parent;

        KeyNode *k1 = (KeyNode *)malloc(sizeof(KeyNode));
        KeyNode *k2 = (KeyNode *)malloc(sizeof(KeyNode));
        k1->size = 1;
        k1->utag = 2;
        k2->size = 1;
        k2->utag = 2;
        int nextpush;

        if(dptr->ParentIndex == 0)
        {
            nextpush = parent->KeyVal[0];

            k2->KeyVal[0] = parent->KeyVal[1];

            k2->ptr.Dataptr[0] = parent->ptr.Dataptr[1];
            k2->ptr.Dataptr[0]->parent = k2;
            k2->ptr.Dataptr[0]->ParentIndex = 0;

            k2->ptr.Dataptr[1] = parent->ptr.Dataptr[2];
            k2->ptr.Dataptr[1]->parent = k2;
            k2->ptr.Dataptr[1]->ParentIndex = 1;

            parent->KeyVal[0] = d2->ItemArr[0]->item_id;
            parent->size = 1;
            parent->ptr.Dataptr[1] = d2;
            parent->ptr.Dataptr[1]->parent = parent;
            parent->ptr.Dataptr[1]->ParentIndex = 1;
            k1 = parent;
        }
        else if(dptr->ParentIndex == 1)
        {
            nextpush = d2->ItemArr[0]->item_id;

            k2->KeyVal[0] = parent->KeyVal[1];

            k2->ptr.Dataptr[0] = d2;
            k2->ptr.Dataptr[0]->parent = k2;
            k2->ptr.Dataptr[0]->ParentIndex = 0;

            k2->ptr.Dataptr[1] = parent->ptr.Dataptr[2];
            k2->ptr.Dataptr[1]->parent = k2;
            k2->ptr.Dataptr[1]->ParentIndex = 1;

            parent->size = 1;
            k1 = parent;
        }
        else if(dptr->ParentIndex == 2)
        {
            nextpush = parent->KeyVal[1];

            k2->KeyVal[0] = d2->ItemArr[0]->item_id;

            k2->ptr.Dataptr[0] = dptr;
            k2->ptr.Dataptr[0]->parent = k2;
            k2->ptr.Dataptr[0]->ParentIndex = 0;

            k2->ptr.Dataptr[1] = d2;
            k2->ptr.Dataptr[1]->parent = k2;
            k2->ptr.Dataptr[1]->ParentIndex = 1;

            parent->size = 1;
            k1 = parent;
        }
        printf("new 2 keyvals splitted are %d and %d and pushing %d\n", k1->KeyVal[0], k2->KeyVal[0], nextpush);
        return PushKeyUp(nextpush, gparent, parent->ParentIndex, k1, k2, origin_root);
    }
}

DataNode* HeadofDoubly(KeyNode* root)
{
    printf("\ninside HeadofD\n");
    if(root->utag == 2)
    {
        return root->ptr.Dataptr[0];
    }
    else
    {
        return HeadofDoubly(root->ptr.Keyptr[0]);
    }
}

void PrintBplus (KeyNode* root)
{
    printf("inside print\n");
    DataNode *head = HeadofDoubly(root);
    printf("\nlets traverse in print\n");
    printf("at root first key is %d\n", root->KeyVal[0]);
    if(root->size == 2)
    {
        printf("at root second key is %d\n", root->KeyVal[1]);
    }
    while(head)
    {
        for(int i = 0; i < head->size; i++)
        {
            printf("item id: %d\n", head->ItemArr[i]->item_id);
        }
        printf("\n");
        head = head->next;
    }
}

Item *CreateItem(int id)
{
    Item *nitem = (Item *)malloc(sizeof(Item));
    nitem->item_id = id;
    return nitem;
}

KeyNode *DeletFromDataNode(int val, DataNode *dptr)
{
    if(dptr != NULL)
    {
        int i;
        for(i = 0; i < dptr->size && dptr->ItemArr[i] != val; i++){}
        if(i = dptr->size)
        {
            printf("No item matches the item id\n");
        }
        else
        {
            i++;
            while(i < dptr->size)
            {
                dptr->ItemArr[i-1] = dptr->ItemArr[i];
                i++;
            }
            dptr->size -= 1;
        }
        if(dptr->size -1 < MINCAPACITY)
        {
            if(dptr->next != NULL && dptr->next->parent == dptr->parent)
            {
                DataNode *temp = dptr->next;
                if(temp->size - 1 > MINCAPACITY)
                {
                    dptr->ItemArr[dptr->size] = temp->ItemArr[0];
                    i = 1;
                    while(i < temp->size)
                    {
                        temp->ItemArr[i-1] = temp->ItemArr[i];
                        i++;    
                    }
                    temp->size -= 1;
                    temp->parent->KeyVal[temp->ParentIndex] = temp->ItemArr[0];
                    dptr->size += 1;
                }
            }
            else
            {
                if(dptr->prev != NULL && dptr->prev->parent == dptr->parent)
                {
                    DataNode *tptr = dptr->prev;
                    if(tptr->size - 1 > MINCAPACITY)
                    {
                        int temp = dptr->size + 1;
                        while(temp > 0)
                        {
                            dptr->ItemArr[temp] = dptr->ItemArr[temp - 1];
                        }
                    }   
                }
            }
        }
    }
}

KeyNode *Delete(KeyNode *root, int val, KeyNode *origin_root)
{
    if(root != NULL)
    {
        for(int i = 0; i < root->size; i++)
        {
            if(val < root->KeyVal[i])
            {
                if(root->utag == 2)
                {
                    printf("Deleting value %d from index %d\n", val, root->KeyVal[i]);
                    return DeleteFromDataNode();
                }
                else
                {
                    printf("Going to the left of %d \n", root->KeyVal[i]);
                     return Delete(root->ptr.Keyptr[i], val, origin_root);
                }
            }
        }
        if(root->utag == 2)
        {
            printf("Deleting value %d from right of %d\n", val, root->KeyVal[root->size-1]);
            return DeleteFromDataNode();
        }
        else
        {
            printf("Going to the rightmost child of %d", root->KeyVal[root->size-1]);
            return Delete(root->ptr.Keyptr[root->size], val, origin_root);
        }
    }
}


int main()
{
    KeyNode *root = initializeBpTree(2);
    root = Insert(root, CreateItem(5), root);
    root = Insert(root, CreateItem(4), root);
    root = Insert(root, CreateItem(6), root);
    root = Insert(root, CreateItem(3), root);
    root = Insert(root, CreateItem(7), root);
    root = Insert(root, CreateItem(1), root);
    root = Insert(root, CreateItem(10), root);
    root = Insert(root, CreateItem(0), root);
    root = Insert(root, CreateItem(11), root);
    root = Insert(root, CreateItem(21), root);
    root = Insert(root, CreateItem(20), root);
    root = Insert(root, CreateItem(15), root);
    root = Insert(root, CreateItem(8), root);
    root = Insert(root, CreateItem(13), root);
    root = Insert(root, CreateItem(17), root);
    root = Insert(root, CreateItem(22), root);
    root = Insert(root, CreateItem(25), root);
    

    PrintBplus(root); 

    return 0;
}

