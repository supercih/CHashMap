
#ifndef _GENERICS_H_
#define _GENERICS_H_

#include "memory.h"

#ifndef BOOL
#define BOOL	int
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN_OUT
#define IN_OUT
#endif

#ifndef OPTIONAL_IN
#define OPTIONAL_IN
#endif

#ifndef OPTIONAL_OUT
#define OPTIONAL_OUT
#endif

#define LIST_INCREASE_SIZE	8

#define GENERICS_LIST_DEFFINE(LIST_NAME, ITEM_TYPE)													\
																									\
	typedef struct _##LIST_NAME##_ {																\
		unsigned int	ItemsCount;																	\
																									\
		unsigned int	ItemsArraySize;																\
		ITEM_TYPE*		ItemsArray;																	\
	} LIST_NAME, *P##LIST_NAME;																		\
																									\
	LIST_NAME LIST_NAME##_Instance(void);															\
																									\
	LIST_NAME LIST_NAME##_Instance_WithCount(unsigned int ExpectedItemsCount);						\
																									\
	BOOL LIST_NAME##_Init(LIST_NAME* pList);														\
																									\
	BOOL LIST_NAME##_Init_WithCount(LIST_NAME* pList, unsigned int ExpectedItemsCount);				\
																									\
	BOOL LIST_NAME##_Insert(LIST_NAME* pList, unsigned int idx, ITEM_TYPE Item);					\
																									\
	BOOL LIST_NAME##_Add(LIST_NAME* pList, ITEM_TYPE Item);											\
																									\
	int LIST_NAME##_IndexOf(LIST_NAME* pList, ITEM_TYPE Item);										\
																									\
	BOOL LIST_NAME##_RemoveIndex(LIST_NAME* pList, unsigned int idx);								\
																									\
	BOOL LIST_NAME##_Remove(LIST_NAME* pList, ITEM_TYPE Item);										\
																									\
	BOOL LIST_NAME##_Contains(LIST_NAME* pList, ITEM_TYPE Item);									\
																									\
	BOOL LIST_NAME##_Clear(LIST_NAME* pList);														\
																									\
	BOOL LIST_NAME##_Destroy(LIST_NAME* pList);														\
																									\
	BOOL LIST_NAME##_GetIndex(LIST_NAME* pList, unsigned int idx, OUT ITEM_TYPE* pItem);			\
																									\
	BOOL LIST_NAME##_GetIndexPtr(LIST_NAME* pList, unsigned int idx, OUT ITEM_TYPE** ppItem);		\
																									\
	BOOL LIST_NAME##_SetIndex(LIST_NAME* pList, unsigned int idx, ITEM_TYPE Item);                  \
																									\
	int LIST_NAME##_Count(LIST_NAME* pList);														\
																									\
	BOOL LIST_NAME##_Sort(LIST_NAME* pList,															\
							int (*ITEM_TYPE##_Compare)(ITEM_TYPE Item1, ITEM_TYPE Itme2));			\



#define GENERICS_LIST_IMPLEMENT(LIST_NAME, ITEM_TYPE)												\
																									\
	LIST_NAME LIST_NAME##_Instance(void) {															\
		LIST_NAME instance = {0};																	\
																									\
		instance.ItemsCount = 0;																	\
																									\
		instance.ItemsArraySize = 0;																\
		instance.ItemsArray = NULL;																	\
																									\
		return instance;																			\
	}																								\
																									\
	LIST_NAME LIST_NAME##_Instance_WithCount(unsigned int ExpectedItemsCount)						\
	{																								\
		LIST_NAME instance = {0};																	\
																									\
		instance.ItemsCount = 0;																	\
																									\
		instance.ItemsArraySize = ExpectedItemsCount;												\
		instance.ItemsArray = AllocateMemory(instance.ItemsArraySize * sizeof(ITEM_TYPE));			\
																									\
		if(instance.ItemsArray == NULL) {															\
			instance.ItemsArraySize = 0;															\
		}																							\
																									\
		return instance;																			\
	}																								\
																									\
	BOOL LIST_NAME##_Init(LIST_NAME* pList)															\
	{																								\
		if(pList == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		memset(pList, 0, sizeof(LIST_NAME));														\
																									\
		pList->ItemsCount = 0;																		\
																									\
		pList->ItemsArraySize = 0;																	\
		pList->ItemsArray = NULL;																	\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_Init_WithCount(LIST_NAME* pList, unsigned int ExpectedItemsCount)				\
	{																								\
		if(pList == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		memset(pList, 0, sizeof(LIST_NAME));														\
																									\
		pList->ItemsCount = 0;																		\
																									\
		pList->ItemsArraySize = ExpectedItemsCount;													\
		pList->ItemsArray = AllocateMemory(pList->ItemsArraySize * sizeof(ITEM_TYPE));				\
																									\
		if(pList->ItemsArray == NULL) {																\
			pList->ItemsArraySize = 0;																\
			return FALSE;																			\
		}																							\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_Insert(LIST_NAME* pList, unsigned int idx, ITEM_TYPE Item)                     \
	{																								\
		unsigned int	i = 0;																		\
		ITEM_TYPE*		pNewArray = NULL;															\
																									\
		if(pList == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		if(idx > pList->ItemsCount) {																\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsArray == NULL) {																\
			pList->ItemsCount = 0;																	\
																									\
			pList->ItemsArray = AllocateMemory(sizeof(ITEM_TYPE) * LIST_INCREASE_SIZE);				\
																									\
			if(pList->ItemsArray == NULL) {															\
				pList->ItemsArraySize = 0;															\
				return FALSE;																		\
			}																						\
																									\
			pList->ItemsArraySize = LIST_INCREASE_SIZE;												\
		}																							\
																									\
		if(pList->ItemsCount >= pList->ItemsArraySize) {											\
			pNewArray = AllocateMemory(sizeof(ITEM_TYPE)											\
										* (pList->ItemsArraySize + LIST_INCREASE_SIZE));			\
																									\
			if(pNewArray == NULL) {																	\
				return FALSE;																		\
			}																						\
																									\
			memcpy(pNewArray, pList->ItemsArray,													\
										sizeof(ITEM_TYPE) * (pList->ItemsArraySize));				\
			pList->ItemsArraySize = pList->ItemsArraySize + LIST_INCREASE_SIZE;						\
																									\
			FreeMemory(pList->ItemsArray);															\
			pList->ItemsArray = pNewArray;															\
		}																							\
																									\
		for(i=pList->ItemsCount; i>idx; i--) {                                                      \
			pList->ItemsArray[i] = pList->ItemsArray[i-1];											\
			memset(&pList->ItemsArray[i-1], 0, sizeof(ITEM_TYPE));									\
		}																							\
																									\
		pList->ItemsArray[idx] = Item;                                                              \
		pList->ItemsCount++;																		\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_Add(LIST_NAME* pList, ITEM_TYPE Item)											\
	{																								\
		return LIST_NAME##_Insert(pList, pList->ItemsCount, Item);									\
	}																								\
																									\
	int LIST_NAME##_IndexOf(LIST_NAME* pList, ITEM_TYPE Item)										\
	{																								\
		unsigned int i = 0;																			\
		int idx = -1;																				\
																									\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return -1;																				\
		}																							\
																									\
		for(i=0; i<pList->ItemsCount; i++) {														\
			unsigned char* ptr1 = (unsigned char*)(&pList->ItemsArray[i]);							\
			unsigned char* ptr2 = (unsigned char*)(&Item);											\
																									\
			if(memcmp(ptr1, ptr2, sizeof(ITEM_TYPE)) == 0) {										\
				idx = i;																			\
				break;																				\
			}																						\
		}																							\
																									\
		return idx;                                                                                 \
	}																								\
																									\
	BOOL LIST_NAME##_RemoveIndex(LIST_NAME* pList, unsigned int idx)								\
	{																								\
		unsigned int i=0;																			\
																									\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsCount == 0 || idx < 0 || idx > pList->ItemsCount - 1) {                      \
			return FALSE;																			\
		} else {																					\
			memset(&pList->ItemsArray[idx], 0, sizeof(ITEM_TYPE));                                  \
		}																							\
																									\
		for(i=idx; i<pList->ItemsCount-1; i++) {													\
			pList->ItemsArray[i] = pList->ItemsArray[i+1];											\
			memset(&pList->ItemsArray[i+1], 0, sizeof(ITEM_TYPE));									\
		}																							\
																									\
		pList->ItemsCount--;																		\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_Remove(LIST_NAME* pList, ITEM_TYPE Item)										\
	{																								\
		int idx = LIST_NAME##_IndexOf(pList, Item);                                                 \
																									\
		if(idx >= 0) {                                                                              \
			return LIST_NAME##_RemoveIndex(pList, idx);                                             \
		} else {																					\
			return FALSE;																			\
		}																							\
	}																								\
																									\
	BOOL LIST_NAME##_Contains(LIST_NAME* pList, ITEM_TYPE Item)										\
	{																								\
		int idx = LIST_NAME##_IndexOf(pList, Item);                                                 \
																									\
		if(idx >= 0) {                                                                              \
			return TRUE;																			\
		} else {																					\
			return FALSE;																			\
		}																							\
	}																								\
																									\
	BOOL LIST_NAME##_Clear(LIST_NAME* pList)														\
	{																								\
		if(pList == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsArray != NULL) {																\
			memset(pList->ItemsArray, 0, sizeof(ITEM_TYPE) * pList->ItemsArraySize);				\
		}																							\
																									\
		pList->ItemsCount = 0;																		\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_Destroy(LIST_NAME* pList)														\
	{																								\
		if(pList == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsArray != NULL) {																\
			FreeMemory(pList->ItemsArray);															\
			pList->ItemsArray = NULL;																\
		}																							\
																									\
		pList->ItemsArraySize = 0;																	\
		pList->ItemsCount = 0;																		\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_GetIndex(LIST_NAME* pList, unsigned int idx, ITEM_TYPE* pItem)                 \
	{																								\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return FALSE;																			\
		}																							\
																									\
		if(pItem == NULL) {																			\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsCount == 0 || idx < 0 || idx > pList->ItemsCount - 1) {                      \
			return FALSE;																			\
		}																							\
																									\
		*pItem = pList->ItemsArray[idx];															\
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_GetIndexPtr(LIST_NAME* pList, unsigned int idx, ITEM_TYPE** ppItem)			\
	{																								\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return FALSE;																			\
		}																							\
																									\
		if(ppItem == NULL) {																		\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsCount == 0 || idx < 0 || idx > pList->ItemsCount - 1) {                      \
			return FALSE;																			\
		}																							\
																									\
		*ppItem = &pList->ItemsArray[idx];                                                          \
																									\
		return TRUE;																				\
	}																								\
																									\
	BOOL LIST_NAME##_SetIndex(LIST_NAME* pList, unsigned int idx, ITEM_TYPE Item)					\
	{																								\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsCount == 0 || idx < 0 || idx > pList->ItemsCount - 1) {                      \
			return FALSE;																			\
		}																							\
																									\
		pList->ItemsArray[idx] = Item;                                                              \
																									\
		return TRUE;																				\
	}																								\
																									\
	int LIST_NAME##_Count(LIST_NAME* pList) {														\
		if(pList == NULL) {																			\
			return 0;																				\
		}																							\
																									\
		return pList->ItemsCount;																	\
	}																								\
																									\
	BOOL LIST_NAME##_Sort(LIST_NAME* pList,															\
		int (*ITEM_TYPE##_Compare)(ITEM_TYPE Item1, ITEM_TYPE Itme2))								\
	{																								\
		unsigned int	i = 0;																		\
		unsigned int	j = 0;																		\
		unsigned int	min = 0;																	\
		ITEM_TYPE		temp;																		\
																									\
		if(ITEM_TYPE##_Compare == NULL) {															\
			return FALSE;																			\
		}																							\
																									\
		if(pList == NULL || pList->ItemsArray == NULL) {											\
			return FALSE;																			\
		}																							\
																									\
		if(pList->ItemsCount < 2) {																	\
			return TRUE;																			\
		}																							\
																									\
		for(i=0; i<pList->ItemsCount-1; i++)														\
		{																							\
			min = i;																				\
			for(j=i+1; j<pList->ItemsCount; j++) {													\
				if(ITEM_TYPE##_Compare(pList->ItemsArray[j], pList->ItemsArray[min]) < 0) {			\
					min = j;																		\
				}																					\
			}																						\
																									\
			if(min != i)																			\
			{																						\
				temp = pList->ItemsArray[min];														\
				pList->ItemsArray[min] = pList->ItemsArray[i];										\
				pList->ItemsArray[i] = temp;														\
			}																						\
		}																							\
																									\
		return TRUE;																				\
	}																								\

#endif
