#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
   void *pNext;
   void *pPrev;
} ListNode;

typedef struct 
{
   void *pHead;  // points to the beginning of the list
   void *pTail;  // points to the end of the list
   int count;    // number of items in the list
   int offset;   // offset of ListNode within the structure
} List;

/* ------------------------------------------------------------------------
   ListInitialize

   Purpose -      Initializes a List type.
   Parameters -   nextPrevOffset - offset from beginning of structure to
                                   the next and previous pointers with
                                   the structure that makes up the nodes.
   Returns -      None
   Side Effects - 
   ----------------------------------------------------------------------- */
static void ListInitialize(List *pList, int nextPrevOffset)
{
    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->count = 0;
    pList->offset = nextPrevOffset;
}


/* ------------------------------------------------------------------------
   ListAddNode

   Purpose -      Adds a node to the end of the list.

   Parameters -   List *pList        -  pointer to the list
                  void *pStructToAdd -  pointer to the structure to add

   Returns -      none
   Side Effects - 
   ----------------------------------------------------------------------- */
static void ListAddNode(List *pList, void *pStructToAdd)
{
    ListNode *pNode = (ListNode*)((char*)pStructToAdd + pList->offset);
    pNode->pNext = NULL;

    if (pList->pTail == NULL) {
        pList->pHead = pList->pTail = pStructToAdd;
    } else {
        ListNode *pTailNode = (ListNode*)((char*)pList->pTail + pList->offset);
        pTailNode->pNext = pStructToAdd;
        pNode->pPrev = pList->pTail;
        pList->pTail = pStructToAdd;
    }

    pList->count++;
}

/* ------------------------------------------------------------------------
   ListRemoveNode

   Purpose -      Removes the specified node from the list.

   Parameters -   List *pList           -  pointer to the list
                  void *pStructToRemove -  pointer to the structure to remove

   Returns -      None

   Side Effects - 
   ----------------------------------------------------------------------- */
static void ListRemoveNode(List *pList, void *pStructToRemove)
{
    ListNode *pNodeToRemove = (ListNode*)((char*)pStructToRemove + pList->offset);

    if (pNodeToRemove->pPrev == NULL) {
        pList->pHead = pNodeToRemove->pNext;
    } else {
        ListNode *pPrevNode = (ListNode*)((char*)pNodeToRemove->pPrev + pList->offset);
        pPrevNode->pNext = pNodeToRemove->pNext;
    }

    if (pNodeToRemove->pNext == NULL) {
        pList->pTail = pNodeToRemove->pPrev;
    } else {
        ListNode *pNextNode = (ListNode*)((char*)pNodeToRemove->pNext + pList->offset);
        pNextNode->pPrev = pNodeToRemove->pPrev;
    }

    pList->count--;
}
/* ------------------------------------------------------------------------
   ListPopNode

   Purpose -      Removes the first node from the list and returns a pointer 
                  to it.

   Parameters -   List *pList         -  pointer to the list
                  
   Returns -      The function returns a pointer to the removed node.

   Side Effects - 
   ----------------------------------------------------------------------- */
static void *ListPopNode(List *pList)
{
    if (pList->count == 0) {
        return NULL;
    }
    
    void *pStructToPop = pList->pHead;
    ListRemoveNode(pList, pStructToPop);
    return pStructToPop;
}

/* ------------------------------------------------------------------------
   List Testing
   ----------------------------------------------------------------------- */
typedef struct test_list
{
    struct test_list *pNextAll;            // All List pNext
    struct test_list *pPrevAll;            // All List pPrev
    struct test_list *pNextEvenOrOdd;      // Even or Odd List pNext
    struct test_list *pPrevEvenOrOdd;      // Even or Odd List pPrev
    int value;
} TestListStructure;

int main()
{
    TestListStructure allPossibleNodes[100];    // array of all possible nodes
    TestListStructure *pTestStruct;
    List listAll, listEvens, listOdds;          // three different lists
    int offsetList2;

    // start with an empty array
    memset(allPossibleNodes, 0, sizeof(allPossibleNodes));

    // calculate the offset of list 2
    offsetList2 = (void *)&allPossibleNodes[0].pNextEvenOrOdd - (void *)&allPossibleNodes[0];
    printf("The offset of the second list is %d bytes.\n", offsetList2);

    ListInitialize(&listAll, 0);
    ListInitialize(&listEvens, offsetList2);
    ListInitialize(&listOdds, offsetList2);

    // generate random values and add to listAll and then either listEven or listOdd
    for (int i=0; i < 10; ++i)
    {
        allPossibleNodes[i].value = rand() % 100;
        ListAddNode(&listAll, &allPossibleNodes[i]);
        if ((allPossibleNodes[i].value % 2) == 0)
        {
            ListAddNode(&listEvens, &allPossibleNodes[i]);
        }
        else
        {
            ListAddNode(&listOdds, &allPossibleNodes[i]);
        }
    }
    
    // display what's in each of the lists
    printf("There are %d even values.\n", listEvens.count);
    while ((pTestStruct = ListPopNode(&listEvens)) != NULL)
    {
        printf("%d\n", pTestStruct->value);
    }
    
    printf("There are %d odd values.\n", listOdds.count);
    while ((pTestStruct = ListPopNode(&listOdds)) != NULL)
    {
        printf("%d\n", pTestStruct->value);
    }

    printf("There are %d total values.\n", listAll.count);
    while ((pTestStruct = ListPopNode(&listAll)) != NULL)
    {
        printf("%d\n", pTestStruct->value);
    }

    // repopulate the listAll
    for (int i=0; i < 10; ++i)
    {
        ListAddNode(&listAll, &allPossibleNodes[i]);
    }
 
    // remove from front
    ListRemoveNode(&listAll, &allPossibleNodes[0]);
    // remove from middle
    ListRemoveNode(&listAll, &allPossibleNodes[4]);
    // remove from end
    ListRemoveNode(&listAll, &allPossibleNodes[9]);

    printf("There are %d values in the test list after removing 0, 4, and 9.\n", listAll.count);
    while ((pTestStruct = ListPopNode(&listAll)) != NULL)
    {
        printf("%d\n", pTestStruct->value);
    }

    return 0;
}