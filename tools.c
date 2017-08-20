
#include "tools.h"

GENERICS_LIST_IMPLEMENT(IntList, int);

/********************************************** List ****************************************/

LIST List_Instance(unsigned int ElementSizeInBytes) {
    LIST instance = {0};
    
    instance.ElementSizeInBytes = ElementSizeInBytes;
    instance.ElementMaxCount = INT_MAX;
    instance.ElementCount = 0;
    
    instance.BufferSizeInBytes = 0;
    instance.Buffer = NULL;
    
    return instance;
}

LIST List_Instance_WithCount(unsigned int ElementSizeInBytes, int ExpectedElementsCount) {
    LIST instance = {0};
    
    instance.ElementSizeInBytes = ElementSizeInBytes;
    instance.ElementMaxCount = INT_MAX;
    instance.ElementCount = 0;
    
    instance.BufferSizeInBytes = ExpectedElementsCount * ElementSizeInBytes;
    instance.Buffer = AllocateMemory(instance.BufferSizeInBytes);
    
    if(instance.Buffer == NULL) {
        instance.BufferSizeInBytes = 0;
    }
    
    return instance;
}

BOOL List_Init(PLIST list, unsigned int ElementSizeInBytes)
{
    memset(list, 0, sizeof(LIST));
    
    if(ElementSizeInBytes <= 0) {
        return FALSE;
    }
    
    list->ElementSizeInBytes = ElementSizeInBytes;
    list->ElementMaxCount = INT_MAX;
    list->ElementCount = 0;
    
    list->BufferSizeInBytes = 0;
    list->Buffer = NULL;
    
    return TRUE;
}

BOOL List_Init_WithCount(PLIST list, unsigned int ElementSizeInBytes, int ExpectedElementsCount)
{
    memset(list, 0, sizeof(LIST));
    
    if(ElementSizeInBytes <= 0 || ExpectedElementsCount < 0) {
        return FALSE;
    }
    
    list->ElementSizeInBytes = ElementSizeInBytes;
    list->ElementMaxCount = INT_MAX;
    list->ElementCount = 0;
    
    list->BufferSizeInBytes = ExpectedElementsCount * ElementSizeInBytes;
    list->Buffer = AllocateMemory(list->BufferSizeInBytes);
    
    if(list->Buffer == NULL) {
        list->BufferSizeInBytes = 0;
        return FALSE;
    }
    
    return TRUE;
}

BOOL List_Set_ElementMaxCount(IN_OUT PLIST pList, int nElementMaxCount)
{
    if(pList == NULL || nElementMaxCount <= 0) {
        return FALSE;
    }
    
    pList->ElementMaxCount = nElementMaxCount;
    
    return TRUE;
}

void* List_Insert(PLIST list, int idx, void* ElementPtr) {
    int             i = 0;
    unsigned char*  newBuffer = NULL;
    void*           aim_ptr = NULL;
    
    if(ElementPtr == NULL || idx < 0 || idx > list->ElementCount) {
        return NULL;
    }
    
    if(list->ElementCount >= list->ElementMaxCount) {
        return NULL;
    }
    
    if(list->Buffer == NULL) {
        list->ElementCount = 0;
        
        list->Buffer = AllocateMemory(list->ElementSizeInBytes * LIST_INCREASE_SIZE);
        
        if(list->Buffer == NULL) {
            list->BufferSizeInBytes = 0;
            return NULL;
        }
        
        list->BufferSizeInBytes = list->ElementSizeInBytes * LIST_INCREASE_SIZE;
    }
    
    if(list->ElementCount * list->ElementSizeInBytes >= list->BufferSizeInBytes) {
        if(list->ElementCount + LIST_INCREASE_SIZE > INT_MAX) {
            return NULL;
        }
        
        newBuffer = AllocateMemory(list->BufferSizeInBytes + list->ElementSizeInBytes * LIST_INCREASE_SIZE);
        
        if(newBuffer == NULL) {
            return NULL;
        }
        
        memcpy(newBuffer, list->Buffer, list->BufferSizeInBytes);
        
        FreeMemory(list->Buffer);
        
        list->BufferSizeInBytes = list->BufferSizeInBytes + list->ElementSizeInBytes * LIST_INCREASE_SIZE;
        list->Buffer = newBuffer;
    }
    
    {
        unsigned char* ptr1 = list->Buffer + list->ElementCount * list->ElementSizeInBytes;
        unsigned char* ptr2 = list->Buffer + (list->ElementCount - 1) * list->ElementSizeInBytes;
        
        for(i=list->ElementCount; i>idx; i--) {
            memcpy(ptr1, ptr2, list->ElementSizeInBytes);
            
            ptr1 -= list->ElementSizeInBytes;
            ptr2 -= list->ElementSizeInBytes;
        }
    }
    
    aim_ptr = list->Buffer + idx * list->ElementSizeInBytes;
    memcpy(aim_ptr, ElementPtr, list->ElementSizeInBytes);
    
    list->ElementCount++;
    
    return aim_ptr;
}

void* List_PlaceHoldAt(IN_OUT PLIST pList, IN int idx)
{
    int             i = 0;
    unsigned char*  newBuffer = NULL;
    
    if(idx < 0 || idx > pList->ElementCount) {
        return NULL;
    }
    
    if(pList->ElementCount >= pList->ElementMaxCount) {
        return NULL;
    }
    
    if(pList->Buffer == NULL) {
        pList->ElementCount = 0;
        
        pList->Buffer = AllocateMemory(pList->ElementSizeInBytes * LIST_INCREASE_SIZE);
        
        if(pList->Buffer == NULL) {
            pList->BufferSizeInBytes = 0;
            return NULL;
        }
        
        pList->BufferSizeInBytes = pList->ElementSizeInBytes * LIST_INCREASE_SIZE;
    }
    
    if(pList->ElementCount * pList->ElementSizeInBytes >= pList->BufferSizeInBytes) {
        if(pList->ElementCount + LIST_INCREASE_SIZE > INT_MAX) {
            return FALSE;
        }
        
        newBuffer = AllocateMemory(pList->BufferSizeInBytes + pList->ElementSizeInBytes * LIST_INCREASE_SIZE);
        
        if(newBuffer == NULL) {
            return NULL;
        }
        
        memcpy(newBuffer, pList->Buffer, pList->BufferSizeInBytes);
        
        FreeMemory(pList->Buffer);
        
        pList->BufferSizeInBytes = pList->BufferSizeInBytes + pList->ElementSizeInBytes * LIST_INCREASE_SIZE;
        pList->Buffer = newBuffer;
    }
    
    {
        unsigned char* ptr1 = pList->Buffer + pList->ElementCount * pList->ElementSizeInBytes;
        unsigned char* ptr2 = pList->Buffer + (pList->ElementCount - 1) * pList->ElementSizeInBytes;
        
        for(i=pList->ElementCount; i>idx; i--) {
            memcpy(ptr1, ptr2, pList->ElementSizeInBytes);
            
            ptr1 -= pList->ElementSizeInBytes;
            ptr2 -= pList->ElementSizeInBytes;
        }
    }
    
    pList->ElementCount++;
    
    return (void*)(pList->Buffer + idx * pList->ElementSizeInBytes);
}

int List_IndexOf(IN_OUT PLIST list, IN void* ElementPtr)
{
    int i=0;
    int idx = -1;
    
    unsigned char* ptr1 = NULL;
    unsigned char* ptr2 = (unsigned char*)ElementPtr;
    
    if(ElementPtr == NULL || list->ElementCount == 0) {
        return -1;
    }
    
    ptr1 = list->Buffer;
    
    for(i=0; i<list->ElementCount; i++) {
        
        if(memcmp(ptr1, ptr2, list->ElementSizeInBytes) == 0) {
            idx = i;
            break;
        }
        
        ptr1 += list->ElementSizeInBytes;
    }
    
    return idx;
}

BOOL List_Remove(IN_OUT PLIST list, IN void* ElementPtr)
{
    int idx = List_IndexOf(list, ElementPtr);
    
    if(idx >= 0) {
        int i = 0;
        
        unsigned char* ptr1 = list->Buffer + idx * list->ElementSizeInBytes;
        unsigned char* ptr2 = list->Buffer + (idx + 1) * list->ElementSizeInBytes;
        
        for(i=idx; i<list->ElementCount-1; i++) {
            memcpy(ptr1, ptr2, list->ElementSizeInBytes);
            
            ptr1 += list->ElementSizeInBytes;
            ptr2 += list->ElementSizeInBytes;
        }
        
        list->ElementCount--;
    }
    
    return TRUE;
}

BOOL List_Contains(IN_OUT PLIST list, IN void* ElementPtr)
{
    int idx = List_IndexOf(list, ElementPtr);
    
    if(idx >= 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL List_RemoveAt(PLIST list, int idx)
{
    if(list->ElementCount == 0 || idx < 0 || idx > list->ElementCount - 1) {
        return FALSE;
    }
    
    {
        int i=0;
        unsigned char* ptr1 = list->Buffer + idx * list->ElementSizeInBytes;
        unsigned char* ptr2 = list->Buffer + (idx + 1) * list->ElementSizeInBytes;
        
        for(i=idx; i<list->ElementCount-1; i++) {
            memcpy(ptr1, ptr2, list->ElementSizeInBytes);
            
            ptr1 += list->ElementSizeInBytes;
            ptr2 += list->ElementSizeInBytes;
        }
    }
    
    list->ElementCount--;
    
    return TRUE;
}

BOOL List_RemoveAtEx(IN_OUT PLIST pList, IN int idx, void (*fnElementFreeFunc)(void* pElement))
{
    void* ptr = NULL;
    
    if(fnElementFreeFunc != NULL && pList->ElementSizeInBytes > 0) {
        if(List_GetPtr(pList, idx, &ptr)) {
            fnElementFreeFunc(ptr);
        }
    }
    
    return List_RemoveAt(pList, idx);
}

BOOL List_Clear(PLIST list)
{
    list->ElementCount = 0;
    
    return TRUE;
}

BOOL List_ClearEx(IN_OUT PLIST pList, void (*fnElementFreeFunc)(void* pElement))
{
    if(fnElementFreeFunc != NULL) {
        unsigned char* ptr = pList->Buffer;
        int i = 0;
        for(i=0; i<pList->ElementCount; i++) {
            fnElementFreeFunc(ptr);
            
            ptr += pList->ElementSizeInBytes;
        }
    }
    
    return List_Clear(pList);
}

void List_Destroy(PLIST list) {
    
    if(list->Buffer != NULL) {
        FreeMemory(list->Buffer);
        list->Buffer = NULL;
    }
    
    list->ElementSizeInBytes = 0;
    
    list->BufferSizeInBytes = 0;
    list->ElementCount = 0;
}

void List_DestroyEx(IN_OUT PLIST pList, void (*fnElementFreeFunc)(void* pElement))
{
    if(fnElementFreeFunc != NULL) {
        unsigned char* ptr = pList->Buffer;
        int i = 0;
        for(i=0; i<pList->ElementCount; i++) {
            fnElementFreeFunc(ptr);
            
            ptr += pList->ElementSizeInBytes;
        }
    }
    
    List_Destroy(pList);
}

BOOL List_Get(PLIST list, int idx, void* ElementPtr)
{
    if(ElementPtr == NULL || list->ElementCount == 0 || idx < 0 || idx > list->ElementCount - 1) {
        return FALSE;
    }
    
    memcpy(ElementPtr, list->Buffer + idx * list->ElementSizeInBytes, list->ElementSizeInBytes);
    
    return TRUE;
}

BOOL List_GetPtr(PLIST list, int idx, void** pElementPtr)
{
    if(pElementPtr == NULL || list->ElementCount == 0 || idx < 0 || idx > list->ElementCount - 1) {
        return FALSE;
    }
    
    *pElementPtr = list->Buffer + idx * list->ElementSizeInBytes;
    
    return TRUE;
}

BOOL List_Set(PLIST list, int idx, void* ElementPtr)
{
    if(ElementPtr == NULL || list->ElementCount == 0 || idx < 0 || idx > list->ElementCount - 1) {
        return FALSE;
    }
    
    memcpy(list->Buffer + idx * list->ElementSizeInBytes, ElementPtr, list->ElementSizeInBytes);
    
    return TRUE;
}

BOOL List_Sort(IN_OUT PLIST pList, int (*fnElementCompare)(void* pElement1, void* pElement2))
{
    int             i = 0;
    int             j = 0;
    unsigned int    min = 0;
    unsigned char*  temp_buffer = NULL;
    
    void*           ptr1 = NULL;
    void*           ptr2 = NULL;
    
    if(fnElementCompare == NULL) {
        return FALSE;
    }
    
    if(pList->ElementCount < 2) {
        return TRUE;
    }
    
    temp_buffer = AllocateMemory(pList->ElementSizeInBytes);
    
    if(temp_buffer == NULL) {
        return FALSE;
    }
    
    for(i=0; i<pList->ElementCount-1; i++)
    {
        min = i;
        for(j=i+1; j<pList->ElementCount; j++) {
            ptr1 = pList->Buffer + pList->ElementSizeInBytes * min;
            ptr2 = pList->Buffer + pList->ElementSizeInBytes * j;
            
            if(fnElementCompare(ptr2, ptr1) < 0) {
                min = j;
            }
        }
        
        if(min != i)
        {
            ptr1 = pList->Buffer + pList->ElementSizeInBytes * min;
            ptr2 = pList->Buffer + pList->ElementSizeInBytes * i;
            
            memcpy(temp_buffer, ptr1, pList->ElementSizeInBytes);
            memcpy(ptr1, ptr2, pList->ElementSizeInBytes);
            memcpy(ptr2, temp_buffer, pList->ElementSizeInBytes);
        }
    }
    
    if(temp_buffer != NULL) {
        FreeMemory(temp_buffer);
    }
    
    return TRUE;
}

/********************************************************************************************/



/******************************************** Queue *****************************************/

#define QUEUE_INCREASE_SIZE			8

QUEUE Queue_Instance(IN unsigned int ElementSizeInBytes)
{
    QUEUE queue = {0};
    
    memset(&queue, 0, sizeof(QUEUE));
    
    queue.ElementSizeInBytes = ElementSizeInBytes;
    
    queue.ElementMaxCount = QUEUE_ELEMENT_MAX_COUNT;
    queue.ElementCount = 0;
    
    queue.ElementHead = 0;
    queue.ElementTail = 0;
    
    queue.BufferSizeInBytes = 0;
    queue.Buffer = NULL;
    
    return queue;
}

QUEUE Queue_Instance_WithCount(IN int ExpectedElementsCount, IN unsigned int ElementSizeInBytes)
{
    QUEUE queue = {0};
    
    memset(&queue, 0, sizeof(QUEUE));
    
    queue.ElementSizeInBytes = ElementSizeInBytes;
    
    queue.ElementMaxCount = QUEUE_ELEMENT_MAX_COUNT;
    queue.ElementCount = 0;
    
    queue.ElementHead = 0;
    queue.ElementTail = 0;
    
    queue.BufferSizeInBytes = ExpectedElementsCount * ElementSizeInBytes;
    queue.Buffer = AllocateMemory(queue.BufferSizeInBytes);
    
    if(queue.Buffer == NULL) {
        queue.BufferSizeInBytes = 0;
    }
    
    return queue;
}

BOOL Queue_Init(IN_OUT PQUEUE pQueue, IN unsigned int ElementSizeInBytes)
{
    memset(pQueue, 0, sizeof(QUEUE));
    
    if(ElementSizeInBytes <= 0) {
        return FALSE;
    }
    
    pQueue->ElementSizeInBytes = ElementSizeInBytes;
    
    pQueue->ElementMaxCount = QUEUE_ELEMENT_MAX_COUNT;
    pQueue->ElementCount = 0;
    
    pQueue->ElementHead = 0;
    pQueue->ElementTail = 0;
    
    pQueue->BufferSizeInBytes = 0;
    pQueue->Buffer = NULL;
    
    return TRUE;
}

BOOL Queue_Init_WithCount(IN_OUT PQUEUE pQueue, IN unsigned int ElementSizeInBytes, IN int ExpectedElementsCount)
{
    memset(pQueue, 0, sizeof(QUEUE));
    
    if(ElementSizeInBytes <= 0 || ExpectedElementsCount < 0) {
        return FALSE;
    }
    
    pQueue->ElementSizeInBytes = ElementSizeInBytes;
    
    pQueue->ElementMaxCount = QUEUE_ELEMENT_MAX_COUNT;
    pQueue->ElementCount = 0;
    
    pQueue->ElementHead = 0;
    pQueue->ElementTail = 0;
    
    pQueue->BufferSizeInBytes = ExpectedElementsCount * ElementSizeInBytes;
    pQueue->Buffer = AllocateMemory(pQueue->BufferSizeInBytes);
    
    if(pQueue->Buffer == NULL) {
        pQueue->BufferSizeInBytes = 0;
        return FALSE;
    }
    
    return TRUE;
}

BOOL Queue_Set_ElementMaxCount(IN_OUT PQUEUE pQueue, int elementMaxCount)
{
    if(pQueue == NULL || elementMaxCount <= 0) {
        return FALSE;
    }
    
    pQueue->ElementMaxCount = elementMaxCount;
    return TRUE;
}

BOOL Queue_Push(IN_OUT PQUEUE pQueue, IN void* ElementPtr)
{
    unsigned char*  newBuffer = NULL;
    
    if(ElementPtr == NULL || pQueue->ElementCount >= pQueue->ElementMaxCount) {
        return FALSE;
    }
    
    if(pQueue->Buffer == NULL) {
        pQueue->ElementCount = 0;
        
        pQueue->Buffer = AllocateMemory(pQueue->ElementSizeInBytes * QUEUE_INCREASE_SIZE);
        
        if(pQueue->Buffer == NULL) {
            pQueue->BufferSizeInBytes = 0;
            return FALSE;
        }
        
        pQueue->BufferSizeInBytes = pQueue->ElementSizeInBytes * QUEUE_INCREASE_SIZE;
    }
    
    if(pQueue->ElementCount * pQueue->ElementSizeInBytes >= pQueue->BufferSizeInBytes) {
        if(pQueue->ElementCount + QUEUE_INCREASE_SIZE > INT_MAX) {
            return FALSE;
        }
        
        newBuffer = AllocateMemory(pQueue->BufferSizeInBytes + pQueue->ElementSizeInBytes * QUEUE_INCREASE_SIZE);
        
        if(newBuffer == NULL) {
            return FALSE;
        }
        
        if(pQueue->ElementTail == pQueue->ElementHead - 1) {
            
            memcpy(newBuffer, pQueue->Buffer, pQueue->ElementSizeInBytes * (pQueue->ElementTail + 1));
            
            memcpy(newBuffer + pQueue->ElementSizeInBytes * (pQueue->ElementTail + 1 + QUEUE_INCREASE_SIZE),
                   pQueue->Buffer + pQueue->ElementSizeInBytes * pQueue->ElementHead,
                   pQueue->ElementSizeInBytes * (pQueue->ElementCount - (pQueue->ElementTail+1)));
            
            pQueue->ElementHead += QUEUE_INCREASE_SIZE;
        } else {
            memcpy(newBuffer, pQueue->Buffer, pQueue->BufferSizeInBytes);
        }
        
        FreeMemory(pQueue->Buffer);
        
        pQueue->BufferSizeInBytes = pQueue->BufferSizeInBytes + pQueue->ElementSizeInBytes * QUEUE_INCREASE_SIZE;
        pQueue->Buffer = newBuffer;
    }
    
    if(pQueue->ElementCount == 0) {
        pQueue->ElementHead = 0;
        pQueue->ElementTail = 0;
    } else {
        pQueue->ElementTail++;
    }
    pQueue->ElementTail = pQueue->ElementTail % (pQueue->BufferSizeInBytes / pQueue->ElementSizeInBytes);
    memcpy(pQueue->Buffer + pQueue->ElementSizeInBytes * pQueue->ElementTail, ElementPtr, pQueue->ElementSizeInBytes);
    
    pQueue->ElementCount++;
    
    return TRUE;
}

BOOL Queue_Pop(IN_OUT PQUEUE pQueue, OUT void* ElementPtr)
{
    if(pQueue->ElementCount == 0) {
        return FALSE;
    }
    
    if(ElementPtr != NULL) {
        memcpy(ElementPtr, pQueue->Buffer + pQueue->ElementSizeInBytes * pQueue->ElementHead, pQueue->ElementSizeInBytes);
    }
    
    pQueue->ElementHead++;
    pQueue->ElementHead = pQueue->ElementHead % (pQueue->BufferSizeInBytes / pQueue->ElementSizeInBytes);
    
    pQueue->ElementCount--;
    if(pQueue->ElementCount == 0) {
        pQueue->ElementHead = 0;
        pQueue->ElementTail = 0;
    }
    
    return TRUE;
}

BOOL Queue_Get(IN PQUEUE pQueue, int idx, OUT void* ElementPtr)
{
    unsigned int localIndex = 0;
    
    if(ElementPtr == NULL || idx < 0 || idx >= pQueue->ElementCount) {
        return FALSE;
    }
    
    if(pQueue->ElementCount == 0) {
        return FALSE;
    }
    
    localIndex = pQueue->ElementHead + idx;
    localIndex = localIndex % (pQueue->BufferSizeInBytes / pQueue->ElementSizeInBytes);
    
    memcpy(ElementPtr, pQueue->Buffer + pQueue->ElementSizeInBytes * localIndex, pQueue->ElementSizeInBytes);
    
    return TRUE;
}

BOOL Queue_GetPtr(IN PQUEUE pQueue, int idx, OUT void** pElementPtr)
{
    unsigned int localIndex = 0;
    
    if(pElementPtr == NULL || idx < 0 || idx >= pQueue->ElementCount) {
        return FALSE;
    }
    
    if(pQueue->ElementCount == 0) {
        return FALSE;
    }
    
    localIndex = pQueue->ElementHead + idx;
    localIndex = localIndex % (pQueue->BufferSizeInBytes / pQueue->ElementSizeInBytes);
    
    (*pElementPtr) = pQueue->Buffer + pQueue->ElementSizeInBytes * localIndex;
    
    return TRUE;
}

BOOL Queue_Set(IN_OUT PQUEUE pQueue, int idx, IN void* ElementPtr)
{
    unsigned localIndex = 0;
    
    if(ElementPtr == NULL || idx < 0 || idx >= pQueue->ElementCount) {
        return FALSE;
    }
    
    if(pQueue->ElementCount == 0) {
        return FALSE;
    }
    
    localIndex = pQueue->ElementHead + idx;
    localIndex = localIndex % (pQueue->BufferSizeInBytes / pQueue->ElementSizeInBytes);
    
    memcpy(pQueue->Buffer + pQueue->ElementSizeInBytes * localIndex, ElementPtr, pQueue->ElementSizeInBytes);
    
    return TRUE;
}

BOOL Queue_Clear(IN_OUT PQUEUE pQueue)
{
    pQueue->ElementHead = 0;
    pQueue->ElementTail = 0;
    pQueue->ElementCount = 0;
    
    return TRUE;
}

BOOL Queue_Destroy(IN_OUT PQUEUE pQueue)
{
    if(pQueue->Buffer != NULL) {
        FreeMemory(pQueue->Buffer);
        pQueue->Buffer = NULL;
    }
    
    pQueue->ElementSizeInBytes = 0;
    pQueue->ElementMaxCount = 0;
    pQueue->ElementCount = 0;
    
    pQueue->ElementHead = 0;
    pQueue->ElementTail = 0;
    
    pQueue->BufferSizeInBytes = 0;
    
    return TRUE;
}

/********************************************************************************************/

static unsigned int DefaultPrimeNumberArray[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

BOOL HashMap_Init(IN_OUT PHASH_MAP pHashMap, IN unsigned int KeySizeInBytes, IN unsigned int ValueSizeInBytes,
                  IN unsigned int PrimeNumberArray[], IN unsigned int PrimeNumberArrayLength)
{
    memset(pHashMap, 0, sizeof(HASH_MAP));
    
    if(KeySizeInBytes <= 0 || ValueSizeInBytes <= 0) {
        return FALSE;
    }
    
    if(PrimeNumberArray == NULL) {
        pHashMap->pRootNode = NULL;
        pHashMap->NodesCount = 0;
        pHashMap->TotalNodeCount = 0;
        
        pHashMap->KeySizeInBytes = KeySizeInBytes;
        pHashMap->ValueSizeInBytes = ValueSizeInBytes;
        
        List_Init(&pHashMap->NodePtrList, sizeof(PHASH_MAP_NODE));
        
        pHashMap->PrimeNumberArrayLength = 0;
        
        pHashMap->PrimeNumberArray = AllocateMemory(sizeof(DefaultPrimeNumberArray));
        
        if(pHashMap->PrimeNumberArray == NULL) {
            return FALSE;
        }
        
        pHashMap->PrimeNumberArrayLength = sizeof(DefaultPrimeNumberArray) / sizeof(unsigned int);
        memcpy(pHashMap->PrimeNumberArray, DefaultPrimeNumberArray, sizeof(DefaultPrimeNumberArray));
        
        pHashMap->pRootNode = AllocateMemory(sizeof(HASH_MAP_NODE));
        if(pHashMap->pRootNode == NULL) {
            return FALSE;
        }
        
        return TRUE;
    } else {
        pHashMap->pRootNode = NULL;
        pHashMap->NodesCount = 0;
        pHashMap->TotalNodeCount = 0;
        
        pHashMap->KeySizeInBytes = KeySizeInBytes;
        pHashMap->ValueSizeInBytes = ValueSizeInBytes;
        
        List_Init(&pHashMap->NodePtrList, sizeof(PHASH_MAP_NODE));
        
        pHashMap->PrimeNumberArrayLength = 0;
        
        if(PrimeNumberArrayLength <= 0) {
            return FALSE;
        }
        
        pHashMap->PrimeNumberArray = AllocateMemory(sizeof(unsigned int) * PrimeNumberArrayLength);
        
        if(pHashMap->PrimeNumberArray == NULL) {
            return FALSE;
        }
        
        pHashMap->PrimeNumberArrayLength = PrimeNumberArrayLength;
        memcpy(pHashMap->PrimeNumberArray, PrimeNumberArray, sizeof(unsigned int) * PrimeNumberArrayLength);
        
        pHashMap->pRootNode = AllocateMemory(sizeof(HASH_MAP_NODE));
        if(pHashMap->pRootNode == NULL) {
            return FALSE;
        }
        
        return TRUE;
    }
}

static unsigned int CalcResidualOfKey(unsigned char* pKey, unsigned int KeySizeInBytes)
{
    unsigned int residual = 0;
    unsigned int i = 0;
    
    for(i=KeySizeInBytes; i>0; i--)
    {
        residual = residual * 256 + pKey[i-1];
//        residual = residual % 99999991;
        residual = residual % INT_MAX;
    }
    
    return residual;
}

void* HashMap_Add(IN_OUT PHASH_MAP pHashMap, IN void* pKey, IN void* pValue)
{
    PHASH_MAP_NODE  pNode = NULL;
    unsigned int    level = 0;
    void*           pRet = NULL;
    
    unsigned int    KeySizeInBytes = 0;
    unsigned int    ValueSizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL || pValue == NULL || pHashMap->NodesCount >= INT_MAX) {
        return NULL;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    ValueSizeInBytes = pHashMap->ValueSizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            pRet = NULL;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        pRet = NULL;
                        break;
                    } else {
                        if(level >= pHashMap->PrimeNumberArrayLength) {
                            if(pNode->pChildMap == NULL) {
                                pNode->pChildMap = AllocateMemory(sizeof(HASH_MAP));
                                if(pNode->pChildMap == NULL) {
                                    pRet = NULL;
                                    break;
                                }
                                if(!HashMap_Init(pNode->pChildMap,
                                                 pHashMap->KeySizeInBytes, pHashMap->ValueSizeInBytes,
                                                 pHashMap->PrimeNumberArray, pHashMap->PrimeNumberArrayLength)) {
                                    pRet = NULL;
                                    break;
                                }
                            }
                            
                            if(pNode->pChildMap != NULL) {
                                pRet = HashMap_Add(pNode->pChildMap, pKey, pValue);
                            } else {
                                pRet = NULL;
                            }
                            
                            break;
                        } else {
                            if(pNode->NextLevelNodes == NULL) {
                                pNode->NextLevelNodes = AllocateMemory(pHashMap->PrimeNumberArray[level] * sizeof(HASH_MAP_NODE));
                                if(pNode->NextLevelNodes == NULL) {
                                    pRet = NULL;
                                    break;
                                }
                                pHashMap->TotalNodeCount += pHashMap->PrimeNumberArray[level];
                            }
                            
                            if(pNode->NextLevelNodes != NULL) {
                                pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[level]];
                                level++;
                            } else {
                                pRet = NULL;
                                break;
                            }
                        }
                    }
                } else {
                    if(level >= pHashMap->PrimeNumberArrayLength) {
                        if(pNode->pChildMap == NULL) {
                            pNode->pChildMap = AllocateMemory(sizeof(HASH_MAP));
                            if(pNode->pChildMap == NULL) {
                                pRet = NULL;
                                break;
                            }
                            if(!HashMap_Init(pNode->pChildMap,
                                             pHashMap->KeySizeInBytes, pHashMap->ValueSizeInBytes,
                                             pHashMap->PrimeNumberArray, pHashMap->PrimeNumberArrayLength)) {
                                pRet = NULL;
                                break;
                            }
                        }
                        
                        if(pNode->pChildMap != NULL) {
                            pRet = HashMap_Add(pNode->pChildMap, pKey, pValue);
                        } else {
                            pRet = NULL;
                        }
                        
                        break;
                    } else {
                        if(pNode->NextLevelNodes == NULL) {
                            pNode->NextLevelNodes =
                            AllocateMemory(pHashMap->PrimeNumberArray[level] * sizeof(HASH_MAP_NODE));
                            if(pNode->NextLevelNodes == NULL) {
                                pRet = NULL;
                                break;
                            }
                            pHashMap->TotalNodeCount += pHashMap->PrimeNumberArray[level];
                        }
                        
                        if(pNode->NextLevelNodes != NULL) {
                            pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[level]];
                            level++;
                        } else {
                            pRet = NULL;
                            break;
                        }
                    }
                }
            } else {
                if(pNode->Key != NULL) {
                    if(pNode->KeySizeInBytes != KeySizeInBytes) {
                        FreeMemory(pNode->Key);
                        pNode->Key = NULL;
                    }
                }
                if(pNode->Key == NULL) {
                    pNode->Key = AllocateMemory(KeySizeInBytes);
                    if(pNode->Key == NULL) {
                        pRet = NULL;
                        break;
                    }
                    pNode->KeySizeInBytes = KeySizeInBytes;
                }
                
                if(pNode->Value != NULL) {
                    if(pNode->ValueSizeInBytes != ValueSizeInBytes) {
                        FreeMemory(pNode->Value);
                        pNode->Value = NULL;
                    }
                }
                if(pNode->Value == NULL) {
                    pNode->Value = AllocateMemory(ValueSizeInBytes);
                    if(pNode->Value == NULL) {
                        pRet = NULL;
                        break;
                    }
                    pNode->ValueSizeInBytes = ValueSizeInBytes;
                }
                
                memcpy(pNode->Key, pKey, KeySizeInBytes);
                memcpy(pNode->Value, pValue, ValueSizeInBytes);
                
                pNode->Level = level;
                pNode->bOccupied = TRUE;
                
                pRet = pNode->Value;
                
                break;
            }
        }
    }
    
    if(pRet != NULL) {
        pHashMap->NodesCount++;
    }
    
    return pRet;
}

void* HashMap_PlaceHold(IN_OUT PHASH_MAP pHashMap, IN void* pKey)
{
    PHASH_MAP_NODE pNode = NULL;
    unsigned int level = 0;
    void* pRet = NULL;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL || pHashMap->NodesCount >= INT_MAX) {
        return NULL;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    ValueSizeInBytes = pHashMap->ValueSizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            pRet = NULL;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        pRet = NULL;
                        break;
                    } else {
                        if(level >= pHashMap->PrimeNumberArrayLength) {
                            if(pNode->pChildMap == NULL) {
                                pNode->pChildMap = AllocateMemory(sizeof(HASH_MAP));
                                if(pNode->pChildMap == NULL) {
                                    pRet = NULL;
                                    break;
                                }
                                if(!HashMap_Init(pNode->pChildMap,
                                                 pHashMap->KeySizeInBytes, pHashMap->ValueSizeInBytes,
                                                 pHashMap->PrimeNumberArray, pHashMap->PrimeNumberArrayLength)) {
                                    pRet = NULL;
                                    break;
                                }
                            }
                            
                            if(pNode->pChildMap != NULL) {
                                pRet = HashMap_PlaceHold(pNode->pChildMap, pKey);
                            } else {
                                pRet = NULL;
                            }
                            
                            break;
                        } else {
                            if(pNode->NextLevelNodes == NULL) {
                                pNode->NextLevelNodes
                                = AllocateMemory(pHashMap->PrimeNumberArray[level] * sizeof(HASH_MAP_NODE));
                                if(pNode->NextLevelNodes == NULL) {
                                    pRet = NULL;
                                    break;
                                }
                                pHashMap->TotalNodeCount += pHashMap->PrimeNumberArray[level];
                            }
                            
                            if(pNode->NextLevelNodes != NULL) {
                                pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[level]];
                                level++;
                            } else {
                                pRet = NULL;
                                break;
                            }
                        }
                    }
                } else {
                    if(level >= pHashMap->PrimeNumberArrayLength) {
                        if(pNode->pChildMap == NULL) {
                            pNode->pChildMap = AllocateMemory(sizeof(HASH_MAP));
                            if(pNode->pChildMap == NULL) {
                                pRet = NULL;
                                break;
                            }
                            if(!HashMap_Init(pNode->pChildMap,
                                             pHashMap->KeySizeInBytes, pHashMap->ValueSizeInBytes,
                                             pHashMap->PrimeNumberArray, pHashMap->PrimeNumberArrayLength)) {
                                pRet = NULL;
                                break;
                            }
                        }
                        
                        if(pNode->pChildMap != NULL) {
                            pRet = HashMap_PlaceHold(pNode->pChildMap, pKey);
                        } else {
                            pRet = NULL;
                        }
                        
                        break;
                    } else {
                        if(pNode->NextLevelNodes == NULL) {
                            pNode->NextLevelNodes
                            = AllocateMemory(pHashMap->PrimeNumberArray[level] * sizeof(HASH_MAP_NODE));
                            if(pNode->NextLevelNodes == NULL) {
                                pRet = NULL;
                                break;
                            }
                            pHashMap->TotalNodeCount += pHashMap->PrimeNumberArray[level];
                        }
                        
                        if(pNode->NextLevelNodes != NULL) {
                            pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[level]];
                            level++;
                        } else {
                            pRet = NULL;
                            break;
                        }
                    }
                }
            } else {
                if(pNode->Key != NULL) {
                    if(pNode->KeySizeInBytes != KeySizeInBytes) {
                        FreeMemory(pNode->Key);
                        pNode->Key = NULL;
                    }
                }
                if(pNode->Key == NULL) {
                    pNode->Key = AllocateMemory(KeySizeInBytes);
                    if(pNode->Key == NULL) {
                        pRet = NULL;
                        break;
                    }
                    pNode->KeySizeInBytes = KeySizeInBytes;
                }
                
                if(pNode->Value != NULL) {
                    if(pNode->ValueSizeInBytes != ValueSizeInBytes) {
                        FreeMemory(pNode->Value);
                        pNode->Value = NULL;
                    }
                }
                if(pNode->Value == NULL) {
                    pNode->Value = AllocateMemory(ValueSizeInBytes);
                    if(pNode->Value == NULL) {
                        pRet = NULL;
                        break;
                    }
                    pNode->ValueSizeInBytes = ValueSizeInBytes;
                }
                
                memcpy(pNode->Key, pKey, KeySizeInBytes);
                
                pNode->Level = level;
                pNode->bOccupied = TRUE;
                
                pRet = pNode->Value;
                
                break;
            }
        }
    }
    
    if(pRet != NULL) {
        pHashMap->NodesCount++;
    }
    
    return pRet;
}

BOOL HashMap_Get(IN PHASH_MAP pHashMap, IN void* pKey, OUT void* pValue)
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int residual = 0;
    unsigned int* pValueSizeInBytes = &ValueSizeInBytes;
    
    if(pKey == NULL || pValue == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    ValueSizeInBytes = pHashMap->ValueSizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        if(pNode->ValueSizeInBytes > *pValueSizeInBytes) {
                            *pValueSizeInBytes = pNode->ValueSizeInBytes;
                            
                            bRet = FALSE;
                            break;
                        } else {
                            *pValueSizeInBytes = pNode->ValueSizeInBytes;
                            
                            if(pValue != NULL) {
                                memcpy(pValue, pNode->Value, pNode->ValueSizeInBytes);
                                bRet = TRUE;
                            } else {
                                bRet = FALSE;
                            }
                            break;
                        }
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_Get(pNode->pChildMap, pKey, pValue);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    return bRet;
}

BOOL HashMap_GetPtr(IN PHASH_MAP pHashMap, IN void* pKey, OUT void** pValuePtr)
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int residual = 0;
    unsigned int* pValueSizeInBytes = &ValueSizeInBytes;
    
    if(pKey == NULL || pValuePtr == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    ValueSizeInBytes = pHashMap->ValueSizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        *pValuePtr = pNode->Value;
                        
                        if(pValueSizeInBytes != NULL) {
                            *pValueSizeInBytes = pNode->ValueSizeInBytes;
                        }
                        
                        bRet = TRUE;
                        break;
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_GetPtr(pNode->pChildMap, pKey, pValuePtr);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    return bRet;
}

BOOL HashMap_Set(IN_OUT PHASH_MAP pHashMap, IN void* pKey, IN void* pValue)
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL || pValue == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    ValueSizeInBytes = pHashMap->ValueSizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        if(pNode->Value != NULL) {
                            if(pNode->ValueSizeInBytes == ValueSizeInBytes) {
                                memcpy(pNode->Value, pValue, ValueSizeInBytes);
                                pNode->ValueSizeInBytes = ValueSizeInBytes;
                                
                                bRet = TRUE;
                                break;
                            } else {
                                FreeMemory(pNode->Value);
                                pNode->Value = NULL;
                                
                                pNode->Value = AllocateMemory(ValueSizeInBytes);
                                if(pNode->Value == NULL) {
                                    bRet = FALSE;
                                    break;
                                }
                                pNode->ValueSizeInBytes = ValueSizeInBytes;
                                memcpy(pValue, pNode->Value, pNode->ValueSizeInBytes);
                                
                                bRet = TRUE;
                                break;
                            }
                        } else {
                            pNode->Value = AllocateMemory(ValueSizeInBytes);
                            if(pNode->Value == NULL) {
                                bRet = FALSE;
                                break;
                            }
                            pNode->ValueSizeInBytes = ValueSizeInBytes;
                            memcpy(pValue, pNode->Value, pNode->ValueSizeInBytes);
                            
                            bRet = TRUE;
                            break;
                        }
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_Set(pNode->pChildMap, pKey, pValue);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    return bRet;
}

BOOL HashMap_Remove(IN_OUT PHASH_MAP pHashMap, IN void* pKey)
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        pNode->bOccupied = FALSE;
                        
                        bRet = TRUE;
                        break;
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_Remove(pNode->pChildMap, pKey);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    if(bRet) {
        pHashMap->NodesCount--;
    }
    
    return bRet;
}


BOOL HashMap_RemoveEx(IN_OUT PHASH_MAP pHashMap, IN void* pKey,
                      void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue))
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        pNode->bOccupied = FALSE;
                        
                        if(fnKeyFreeFunc != NULL) {
                            fnKeyFreeFunc(pNode->Key);
                        }
                        
                        if(fnValueFreeFunc != NULL) {
                            fnValueFreeFunc(pNode->Value);
                        }
                        
                        bRet = TRUE;
                        break;
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_Remove(pNode->pChildMap, pKey);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    if(bRet) {
        pHashMap->NodesCount--;
    }
    
    return bRet;
}

BOOL HashMap_ContainsKey(IN PHASH_MAP pHashMap, IN void* pKey)
{
    PHASH_MAP_NODE pNode = NULL;
    BOOL bRet = FALSE;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int residual = 0;
    
    if(pKey == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pHashMap->KeySizeInBytes;
    
    residual = CalcResidualOfKey(pKey, KeySizeInBytes);
    
    pNode = pHashMap->pRootNode;
    
    while(TRUE) {
        if(pNode == NULL) {
            bRet = FALSE;
            break;
        } else {
            if(pNode->bOccupied) {
                if(pNode->KeySizeInBytes == KeySizeInBytes) {
                    if(memcmp(pNode->Key, pKey, pHashMap->KeySizeInBytes) == 0) {
                        bRet = TRUE;
                        break;
                    }
                }
            }
            
            if(!bRet) {
                if(pNode->NextLevelNodes != NULL) {
                    pNode = &pNode->NextLevelNodes[residual % pHashMap->PrimeNumberArray[pNode->Level]];
                } else if(pNode->pChildMap != NULL) {
                    bRet = HashMap_ContainsKey(pNode->pChildMap, pKey);
                    break;
                } else {
                    bRet = FALSE;
                    break;
                }
            }
        }
    }
    
    return bRet;
}

//
// through preorder traversal
//
BOOL HashMap_Clear(IN_OUT PHASH_MAP pHashMap)
{
    PHASH_MAP_NODE pNode = NULL;
    int i = 0;
    
    if(pHashMap->pRootNode == NULL) {
        return TRUE;
    }
    
    List_Clear(&pHashMap->NodePtrList);
    List_Add(&pHashMap->NodePtrList, &pHashMap->pRootNode);
    
    while(TRUE) {
        if(List_Count(&pHashMap->NodePtrList) > 0) {
            List_Get(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList) - 1, &pNode);
            List_RemoveAt(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList) - 1);
        } else {
            break;
        }
        
        pNode->bOccupied = FALSE;
        
        if(pNode->pChildMap != NULL) {
            HashMap_Clear(pNode->pChildMap);
        }
        
        if(pNode->NextLevelNodes != NULL) {
            PHASH_MAP_NODE pTemp = NULL;
            
            for(i=pHashMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                pTemp = &(pNode->NextLevelNodes[i]);
                
                List_Add(&pHashMap->NodePtrList, &pTemp);
            }
        }
    }
    
    pHashMap->NodesCount = 0;
    
    return TRUE;
}

BOOL HashMap_ClearEx(IN_OUT PHASH_MAP pHashMap,
                     void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue))
{
    PHASH_MAP_NODE pNode = NULL;
    int i = 0;
    
    if(pHashMap->pRootNode == NULL) {
        return TRUE;
    }
    
    List_Clear(&pHashMap->NodePtrList);
    List_Add(&pHashMap->NodePtrList, &pHashMap->pRootNode);
    
    while(TRUE) {
        if(List_Count(&pHashMap->NodePtrList) > 0) {
            List_Get(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList) - 1, &pNode);
            List_RemoveAt(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList) - 1);
        } else {
            break;
        }
        
        if(pNode->bOccupied) {
            if(fnKeyFreeFunc != NULL) {
                fnKeyFreeFunc(pNode->Key);
            }
            
            if(fnValueFreeFunc != NULL) {
                fnValueFreeFunc(pNode->Value);
            }
        }
        
        pNode->bOccupied = FALSE;
        
        if(pNode->pChildMap != NULL) {
            HashMap_ClearEx(pNode->pChildMap, fnValueFreeFunc, fnValueFreeFunc);
        }
        
        if(pNode->NextLevelNodes != NULL) {
            PHASH_MAP_NODE pTemp = NULL;
            
            for(i=pHashMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                pTemp = &(pNode->NextLevelNodes[i]);
                
                List_Add(&pHashMap->NodePtrList, &pTemp);
            }
        }
    }
    
    pHashMap->NodesCount = 0;
    
    return TRUE;
}

//
// through postorder traversal
//
void HashMap_Destroy(IN_OUT PHASH_MAP pHashMap)
{
    PHASH_MAP_NODE pNode = NULL;
    int i = 0;
    
    if(pHashMap->pRootNode == NULL) {
        goto EXIT;
    }
    
    List_Clear(&pHashMap->NodePtrList);
    List_Add(&pHashMap->NodePtrList, &pHashMap->pRootNode);
    
    while(TRUE) {
        if(List_Count(&pHashMap->NodePtrList) > 0) {
            List_Get(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList) - 1, &pNode);
        } else {
            break;
        }
        
        if(pNode->pChildMap != NULL) {
            HashMap_Destroy(pNode->pChildMap);
            FreeMemory(pNode->pChildMap);
            pNode->pChildMap = NULL;
        }
        
        if(pNode->NextLevelNodes != NULL) {
            PHASH_MAP_NODE pTemp = NULL;
            BOOL bFlag = TRUE;
            
            for(i=pHashMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                pTemp = &(pNode->NextLevelNodes[i]);
                
                if(pTemp->pChildMap != NULL) {
                    HashMap_Destroy(pTemp->pChildMap);
                    FreeMemory(pTemp->pChildMap);
                    pTemp->pChildMap = NULL;
                }
                
                if(pTemp->NextLevelNodes != NULL) {
                    List_Add(&pHashMap->NodePtrList, &pTemp);
                    bFlag = FALSE;
                } else {
                    if(pTemp->Key != NULL) {
                        FreeMemory(pTemp->Key);
                        pTemp->Key = NULL;
                    }
                    if(pTemp->Value != NULL) {
                        FreeMemory(pTemp->Value);
                        pTemp->Value = NULL;
                    }
                }
            }
            
            if(bFlag) {
                FreeMemory(pNode->NextLevelNodes);
                pNode->NextLevelNodes = NULL;
                
                if(pNode->Key != NULL) {
                    FreeMemory(pNode->Key);
                    pNode->Key = NULL;
                }
                if(pNode->Value != NULL) {
                    FreeMemory(pNode->Value);
                    pNode->Value = NULL;
                }
                List_RemoveAt(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList)-1);
            }
        } else {
            if(pNode->Key != NULL) {
                FreeMemory(pNode->Key);
                pNode->Key = NULL;
            }
            if(pNode->Value != NULL) {
                FreeMemory(pNode->Value);
                pNode->Value = NULL;
            }
            List_RemoveAt(&pHashMap->NodePtrList, List_Count(&pHashMap->NodePtrList)-1);
        }
    }
    
EXIT:
    if(pHashMap->PrimeNumberArray != NULL) {
        FreeMemory(pHashMap->PrimeNumberArray);
        pHashMap->PrimeNumberArray = NULL;
    }
    
    if(pHashMap->pRootNode != NULL) {
        FreeMemory(pHashMap->pRootNode);
        pHashMap->pRootNode = NULL;
    }
    
    List_Destroy(&pHashMap->NodePtrList);
    
    pHashMap->NodesCount = 0;
    pHashMap->PrimeNumberArray = NULL;
    pHashMap->PrimeNumberArrayLength = 0;
    pHashMap->KeySizeInBytes = 0;
    pHashMap->ValueSizeInBytes = 0;
    
    return;
}

//
void HashMap_DestroyEx(IN_OUT PHASH_MAP pHashMap, void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue))
{
    HASH_MAP_ITERATOR	Iterator = {0};
    
    void*				pKey = NULL;
    void*				pValue = NULL;
    
    if(fnKeyFreeFunc != NULL || fnValueFreeFunc != NULL) {
        if(HashMap_Iterator_Init(&Iterator, pHashMap)) {
            while(HashMap_Iterator_GetNextPtr(&Iterator, &pKey, &pValue)) {
                if(fnKeyFreeFunc != NULL) {
                    fnKeyFreeFunc(pKey);
                }
                
                if(fnValueFreeFunc != NULL) {
                    fnValueFreeFunc(pValue);
                }
            }
            HashMap_Iterator_Destroy(&Iterator);
        }
    }
    
    HashMap_Destroy(pHashMap);
}

BOOL HashMap_Iterator_Init(IN PHASH_MAP_ITERATOR pIterator, IN PHASH_MAP pHashMap)
{
    BOOL bRet = TRUE;
    
    memset(pIterator, 0, sizeof(HASH_MAP_ITERATOR));
    
    if(pHashMap == NULL) {
        return FALSE;
    }
    
    pIterator->pMap = pHashMap;
    pIterator->pChildIterator = NULL;
    pIterator->Counter = 0;
    
    bRet = List_Init(&pIterator->NodePtrList, sizeof(PHASH_MAP_NODE));
    
    if(bRet) {
        if(pIterator->pMap->pRootNode != NULL) {
            bRet = (NULL != List_Add(&pIterator->NodePtrList, &pIterator->pMap->pRootNode));
        }
    }
    
    return bRet;
}

BOOL HashMap_Iterator_Reset(IN PHASH_MAP_ITERATOR pIterator)
{
    BOOL bRet = TRUE;
    
    if(pIterator->pChildIterator != NULL) {
        HashMap_Iterator_Destroy(pIterator->pChildIterator);
        FreeMemory(pIterator->pChildIterator);
        pIterator->pChildIterator = NULL;
    }
    
    pIterator->Counter = 0;
    bRet = List_Clear(&pIterator->NodePtrList);
    
    if(bRet) {
        if(pIterator->pMap->pRootNode != NULL) {
            bRet = (NULL != List_Add(&pIterator->NodePtrList, &pIterator->pMap->pRootNode));
        }
    }
    
    return bRet;
}

BOOL HashMap_Iterator_Destroy(IN PHASH_MAP_ITERATOR pIterator)
{
    if(pIterator->pChildIterator != NULL) {
        HashMap_Iterator_Destroy(pIterator->pChildIterator);
        FreeMemory(pIterator->pChildIterator);
        pIterator->pChildIterator = NULL;
    }
    
    List_Destroy(&pIterator->NodePtrList);
    
    pIterator->Counter = 0;
    pIterator->pMap = NULL;
    
    return TRUE;
}

//
// through preorder traversal
//
BOOL HashMap_Iterator_GetNext(IN PHASH_MAP_ITERATOR pIterator, OUT void* pKey, OUT void* pValue)
{
    PHASH_MAP_NODE pNode = NULL;
    int i = 0;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int* pKeySizeInBytes = &KeySizeInBytes;
    unsigned int* pValueSizeInBytes = &ValueSizeInBytes;
    
    if(pKey == NULL || pValue == NULL) {
        return FALSE;
    }
    
    KeySizeInBytes = pIterator->pMap->KeySizeInBytes;
    ValueSizeInBytes = pIterator->pMap->ValueSizeInBytes;
    
    while(TRUE) {
        if(pIterator->Counter >= pIterator->pMap->NodesCount) {
            return FALSE;
        }
        
        if(pIterator->pChildIterator != NULL) {
            if(HashMap_Iterator_GetNext(pIterator->pChildIterator, pKey, pValue)) {
                pIterator->Counter++;
                return TRUE;
            } else {
                if(List_Count(&pIterator->pChildIterator->NodePtrList) <= 0) {
                    HashMap_Iterator_Destroy(pIterator->pChildIterator);
                    FreeMemory(pIterator->pChildIterator);
                    pIterator->pChildIterator = NULL;
                } else {
                    return FALSE;
                }
            }
        }
        
        if(List_Count(&pIterator->NodePtrList) > 0) {
            List_Get(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1, &pNode);
        } else {
            return FALSE;
        }
        
        if(pNode->bOccupied) {
            if(pNode->KeySizeInBytes > *pKeySizeInBytes) {
                *pKeySizeInBytes = pNode->KeySizeInBytes;
                *pValueSizeInBytes = pNode->ValueSizeInBytes;
                return FALSE;
            }
            
            if(pNode->ValueSizeInBytes > *pValueSizeInBytes) {
                *pKeySizeInBytes = pNode->KeySizeInBytes;
                *pValueSizeInBytes = pNode->ValueSizeInBytes;
                return FALSE;
            }
            
            if(pKey == NULL || pValue == NULL) {
                return FALSE;
            } else {
                memcpy(pKey, pNode->Key, pNode->KeySizeInBytes);
                *pKeySizeInBytes = pNode->KeySizeInBytes;
                
                memcpy(pValue, pNode->Value, pNode->ValueSizeInBytes);
                *pValueSizeInBytes = pNode->ValueSizeInBytes;
                
                List_RemoveAt(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1);
                
                if(pNode->pChildMap != NULL) {
                    pIterator->pChildIterator = AllocateMemory(sizeof(HASH_MAP_ITERATOR));
                    HashMap_Iterator_Init(pIterator->pChildIterator, pNode->pChildMap);
                }
                
                if(pNode->NextLevelNodes != NULL) {
                    PHASH_MAP_NODE pTemp = NULL;
                    
                    for(i=pIterator->pMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                        pTemp = &(pNode->NextLevelNodes[i]);
                        List_Add(&pIterator->NodePtrList, &pTemp);
                    }
                }
                
                pIterator->Counter++;
                return TRUE;
            }
        } else {
            List_RemoveAt(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1);
            
            if(pNode->pChildMap != NULL) {
                pIterator->pChildIterator = AllocateMemory(sizeof(HASH_MAP_ITERATOR));
                HashMap_Iterator_Init(pIterator->pChildIterator, pNode->pChildMap);
            }
            
            if(pNode->NextLevelNodes != NULL) {
                PHASH_MAP_NODE pTemp = NULL;
                
                for(i=pIterator->pMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                    pTemp = &(pNode->NextLevelNodes[i]);
                    List_Add(&pIterator->NodePtrList, &pTemp);
                }
            }
        }
    }
}

//
// through preorder traversal
//
BOOL HashMap_Iterator_GetNextPtr(IN PHASH_MAP_ITERATOR pIterator, OUT void** pKeyPtr, OUT void** pValuePtr)
{
    PHASH_MAP_NODE pNode = NULL;
    int i = 0;
    
    unsigned int KeySizeInBytes = 0;
    unsigned int ValueSizeInBytes = 0;
    unsigned int* pKeySizeInBytes = &KeySizeInBytes;
    unsigned int* pValueSizeInBytes = &ValueSizeInBytes;
    
    if((pKeyPtr == NULL && pValuePtr == NULL)) {
        return FALSE;
    }
    
    KeySizeInBytes = pIterator->pMap->KeySizeInBytes;
    ValueSizeInBytes = pIterator->pMap->ValueSizeInBytes;
    
    while(TRUE) {
        if(pIterator->Counter >= pIterator->pMap->NodesCount) {
            return FALSE;
        }
        
        if(pIterator->pChildIterator != NULL) {
            if(HashMap_Iterator_GetNext(pIterator->pChildIterator, pKeyPtr, pValuePtr)) {
                pIterator->Counter++;
                return TRUE;
            } else {
                if(List_Count(&pIterator->pChildIterator->NodePtrList) <= 0) {
                    HashMap_Iterator_Destroy(pIterator->pChildIterator);
                    FreeMemory(pIterator->pChildIterator);
                    pIterator->pChildIterator = NULL;
                } else {
                    return FALSE;
                }
            }
        }
        
        if(List_Count(&pIterator->NodePtrList) > 0) {
            List_Get(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1, &pNode);
        } else {
            return FALSE;
        }
        
        if(pNode->bOccupied) {
            if(pKeyPtr != NULL) {
                *pKeyPtr = pNode->Key;
            }
            
            if(pValuePtr != NULL) {
                *pValuePtr = pNode->Value;
            }
            
            if(pKeySizeInBytes != NULL) {
                *pKeySizeInBytes = pNode->KeySizeInBytes;
            }
            
            if(pValueSizeInBytes != NULL) {
                *pValueSizeInBytes = pNode->ValueSizeInBytes;
            }
            
            List_RemoveAt(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1);
            
            if(pNode->pChildMap != NULL) {
                pIterator->pChildIterator = AllocateMemory(sizeof(HASH_MAP_ITERATOR));
                HashMap_Iterator_Init(pIterator->pChildIterator, pNode->pChildMap);
            }
            
            if(pNode->NextLevelNodes != NULL) {
                PHASH_MAP_NODE pTemp = NULL;
                
                for(i=pIterator->pMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                    pTemp = &(pNode->NextLevelNodes[i]);
                    List_Add(&pIterator->NodePtrList, &pTemp);
                }
            }
            
            pIterator->Counter++;
            return TRUE;
        } else {
            List_RemoveAt(&pIterator->NodePtrList, List_Count(&pIterator->NodePtrList)-1);
            
            if(pNode->pChildMap != NULL) {
                pIterator->pChildIterator = AllocateMemory(sizeof(HASH_MAP_ITERATOR));
                HashMap_Iterator_Init(pIterator->pChildIterator, pNode->pChildMap);
            }
            
            if(pNode->NextLevelNodes != NULL) {
                PHASH_MAP_NODE pTemp = NULL;
                
                for(i=pIterator->pMap->PrimeNumberArray[pNode->Level] - 1; i>=0; i--) {
                    pTemp = &(pNode->NextLevelNodes[i]);
                    List_Add(&pIterator->NodePtrList, &pTemp);
                }
            }
        }
    }
}
