
#ifndef UI_TOOLS_H_
#define UI_TOOLS_H_

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define AllocateMemory(SizeInBytes)     calloc(SizeInBytes, sizeof(char))
#define FreeMemory(Ptr)                 free(Ptr)

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

#ifndef BOOL
#define BOOL    int
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef LIST_INCREASE_SIZE
#define LIST_INCREASE_SIZE  (8)
#endif

//#define TYPE_CHECK

/************************************************ Converter ***************************************************/

typedef union _FLOAT_TO_BYTES_CONVERTER {
    float           FloatValue;
    unsigned char   Bytes[sizeof(float)];
} FLOAT_TO_BYTES_CONVERTER;

typedef union _DOUBLE_TO_BYTES_CONVERTER {
    double          DoubleValue;
    unsigned char   Bytes[sizeof(double)];
} DOUBLE_TO_BYTES_CONVERTER;

typedef union _CHAR_TO_BYTES_CONVERTER {
    char            CharValue;
    unsigned char   Bytes[sizeof(char)];
} CHAR_TO_BYTES_CONVERTER;

typedef union _BYTE_TO_BYTES_CONVERTER {
    unsigned char   ByteValue;
    unsigned char   Bytes[sizeof(char)];
} BYTE_TO_BYTES_CONVERTER;

typedef union _INT_TO_BYTES_CONVERTER {
    int             IntValue;
    unsigned char   Bytes[sizeof(int)];
} INT_TO_BYTES_CONVERTER;

typedef union _SHORT_TO_BYTES_CONVERTER {
    short           ShortValue;
    unsigned char   Bytes[sizeof(short)];
} SHORT_TO_BYTES_CONVERTER;

typedef union _LONG_TO_BYTES_CONVERTER {
    long            LongValue;
    unsigned char   Bytes[sizeof(long)];
} LONG_TO_BYTES_CONVERTER;

typedef union _LONGLONG_TO_BYTES_CONVERTER {
    long long       LonglongValue;
    unsigned char   Bytes[sizeof(long long)];
} LONGLONG_TO_BYTES_CONVERTER;

/*************************************************************************************************************/

/*************************************************** List ****************************************************/
typedef struct _LIST_ {
    unsigned int    ElementSizeInBytes;
    int             ElementMaxCount;
    int             ElementCount;

    unsigned int    BufferSizeInBytes;
    unsigned char*  Buffer;
} LIST, *PLIST;

LIST List_Instance(IN unsigned int ElementSizeInBytes);

LIST List_Instance_WithCount(IN unsigned int ElementSizeInBytes, IN int ExpectedElementsCount);

BOOL List_Init(IN_OUT PLIST pList, IN unsigned int ElementSizeInBytes);

BOOL List_Init_WithCount(IN_OUT PLIST pList, IN unsigned int ElementSizeInBytes, IN int ExpectedElementsCount);

BOOL List_Set_ElementMaxCount(IN_OUT PLIST pList, int elementMaxCount);

void* List_Insert(IN_OUT PLIST pList, IN int idx, IN void* ElementPtr);

static inline void* List_Add(IN_OUT PLIST pList, IN void* ElementPtr)
{
    return List_Insert(pList, pList->ElementCount, ElementPtr);
}

void* List_PlaceHoldAt(IN_OUT PLIST pList, IN int idx);

static inline void* List_PlaceHold(IN_OUT PLIST pList)
{
    return List_PlaceHoldAt(pList, pList->ElementCount);
}

BOOL List_Clear(IN_OUT PLIST pList);

BOOL List_ClearEx(IN_OUT PLIST pList, void (*fnElementFreeFunc)(void* pElement));

void List_Destroy(IN_OUT PLIST pList);

void List_DestroyEx(IN_OUT PLIST pList, void (*fnElementFreeFunc)(void* pElement));

BOOL List_Get(IN PLIST pList, IN int idx, OUT void* ElementPtr);

BOOL List_GetPtr(IN PLIST pList, IN int idx, OUT void** pElementPtr);

BOOL List_Set(IN_OUT PLIST pList, IN int idx, IN void* ElementPtr);

BOOL List_Remove(IN_OUT PLIST pList, IN void* ElementPtr);

BOOL List_RemoveAt(IN_OUT PLIST pList, IN int idx);

BOOL List_RemoveAtEx(IN_OUT PLIST pList, IN int idx, void (*fnElementFreeFunc)(void* pElement));

BOOL List_Contains(IN_OUT PLIST pList, IN void* ElementPtr);

int List_IndexOf(IN_OUT PLIST pList, IN void* ElementPtr);

static inline int List_Count(PLIST list)
{
    return list->ElementCount;
}

BOOL List_Sort(IN_OUT PLIST pList, int (*fnElementCompare)(void* pElement1, void* pElement2));

#define List_Declare_T(T)						{sizeof(T), INT_MAX, 0, 0, NULL, NULL}

#ifdef TYPE_CHECK

#define List_Init_T(T, pList, bResult)												\
    {																				\
        PLIST __l_plist__ = (pList);												\
        (bResult) = List_Init(__l_plist__, sizeof(T));								\
    }																				\


#define List_Init_WithCount_T(T, pList, ExpectedCount, bResult)						\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_expect_cnt__ = (ExpectedCount);							\
        (bResult) = List_Init_WithCount(__l_plist__, sizeof(T), __l_expect_cnt__);	\
    }																				\


#define List_Add_T(T, pList, Data, bResult)											\
    {																				\
        PLIST __l_plist__ = (pList);												\
        T __l_data__ = (Data);														\
        (bResult) = List_Add(__l_plist__, &__l_data__);								\
    }																				\


#define List_Insert_T(T, pList, Index, Data, bResult)								\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T __l_data__ = (Data);														\
        (bResult) = List_Insert(__l_plist__, __l_index__, &__l_data__);				\
    }																				\


#define List_PlaceHold_T(T, pList, ReturnPtr)										\
    {																				\
        PLIST __l_plist__ = (pList);												\
        T*  __l_p_t__ = List_PlaceHold(__l_plist__);								\
        (ReturnPtr) = __l_p_t__;													\
    }																				\


#define List_PlaceHoldAt_T(T, pList, Index, ReturnPtr)								\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T*  __l_p_t__ = List_PlaceHoldAt(__l_plist__, __l_index__);					\
        (ReturnPtr) = __l_p_t__;													\
    }																				\


#define List_Remove_T(T, pList, Data, bResult)										\
    {																				\
        PLIST __l_plist__ = (pList);												\
        T __l_data__ = (Data);														\
        (bResult) = List_Remove(__l_plist__, &__l_data__);							\
    }																				\


#define List_Get_T(T, pList, Index, ReturnData, bResult)							\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T __l_data__;																\
        BOOL __l_bool__ = List_Get(__l_plist__, __l_index__, &__l_data__);			\
        if(__l_bool__) {															\
            (ReturnData) = __l_data__;												\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define List_GetPtr_T(T, pList, Index, ReturnPtr, bResult)							\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T* __l_p_data__ = NULL;														\
        BOOL __l_bool__ = List_GetPtr(__l_plist__, __l_index__,						\
                                    (void**)(&__l_p_data__));						\
        if(__l_bool__) {															\
            (ReturnPtr) = __l_p_data__;												\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define List_Set_T(T, pList, Index, Data, bResult)									\
    {																				\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T __l_data__ = (Data);														\
        (bResult) = List_Set(__l_plist__, __l_index__, &__l_data__);				\
    }																				\


#define List_Contains_T(T, pList, Data, bResult)									\
    {																				\
        PLIST __l_plist__ = (pList);												\
        T __l_data__ = (Data);														\
        (bResult) = List_Contains(__l_plist__, &__l_data__);						\
    }																				\


#define List_IndexOf_T(T, pList, Data, ReturnIndex)									\
    {																				\
        PLIST __l_plist__ = (pList);												\
        T __l_data__ = (Data);														\
        (ReturnIndex) = List_IndexOf(__l_plist__, &__l_data__);						\
    }																				\

#define List_Sort_T(T, pList, fnElementCompareFunc, bResult)						\
    {																				\
        int (*__fn_t_compare__)(T a, T b);											\
        PLIST __l_plist__ = (pList);												\
        __fn_t_compare__ = (fnElementCompareFunc);									\
                                                                                    \
        if(__l_plist__ && __fn_t_compare__ != NULL) {								\
            if(List_Count(__l_plist__) < 2) {										\
                (bResult) = TRUE;													\
            } else {																\
                int		__i__ = 0;													\
                int		__j__ = 0;													\
                int		__min__ = 0;												\
                T		__a__;														\
                T		__b__;														\
                                                                                    \
                for(__i__=0; __i__<List_Count(__l_plist__)-1; __i__++) {			\
                    __min__ = __i__;												\
                    for(__j__=__i__+1; __j__<List_Count(__l_plist__); __j__++) {	\
                        if(!List_Get(__l_plist__, __min__, &__a__)) {				\
                            continue;												\
                        }															\
                        if(!List_Get(__l_plist__, __j__, &__b__)) {					\
                            continue;												\
                        }															\
                                                                                    \
                        if(__fn_t_compare__(__b__, __a__) < 0) {					\
                            __min__ = __j__;										\
                        }															\
                    }																\
                    if(__min__ != __i__) {											\
                        List_Get(__l_plist__, __min__, &__a__);						\
                        List_Get(__l_plist__, __i__, &__b__);						\
                                                                                    \
                        List_Set(__l_plist__, __min__, &__b__);						\
                        List_Set(__l_plist__, __i__, &__a__);						\
                    }																\
                }																	\
                                                                                    \
                (bResult) = TRUE;													\
            }																		\
        } else {																	\
            (bResult) = FALSE;														\
        }																			\
    }																				\


#define List_ClearEx_T(T, pList, fnElementFreeFunc)									\
    {																				\
        void (*__fn_t_free__)(T* a);												\
        PLIST __l_plist__ = (pList);												\
        __fn_t_free__ = (fnElementFreeFunc);										\
                                                                                    \
        if(__l_plist__ && __fn_t_free__ != NULL) {									\
            if(List_Count(__l_plist__) > 0) {										\
                int __i__ = 0;														\
                T*	__t_ptr__ = NULL;												\
                for(__i__=0; __i__<List_Count(__l_plist__); __i__++) {				\
                    if(!List_GetPtr(__l_plist__, __i__,								\
                                            (void**)(&__t_ptr__))) {				\
                        continue;													\
                    }																\
                                                                                    \
                    __fn_t_free__(__t_ptr__);										\
                }																	\
            }																		\
        }																			\
                                                                                    \
        List_Clear(__l_plist__);													\
    }																				\


#define List_RemoveAtEx_T(T, pList, Index, fnElementFreeFunc)						\
    {																				\
        void (*__fn_t_free__)(T* a);												\
        PLIST __l_plist__ = (pList);												\
        unsigned int __l_index__ = (Index);											\
        T* __l_p_data__ = NULL;														\
        __fn_t_free__ = (fnElementFreeFunc);										\
        if(__fn_t_free__ != NULL													\
            && List_GetPtr(__l_plist__, __l_index__, (void**)(&__l_p_data__))) {	\
            __fn_t_free__(__l_p_data__);											\
        }																			\
        List_RemoveAt(__l_plist__, __l_index__);									\
    }																				\


#define List_DestroyEx_T(T, pList, fnElementFreeFunc)								\
    {																				\
        void (*__fn_t_free__)(T* a);												\
        PLIST __l_plist__ = (pList);												\
        __fn_t_free__ = (fnElementFreeFunc);										\
                                                                                    \
        if(__l_plist__ && __fn_t_free__ != NULL) {									\
            if(List_Count(__l_plist__) > 0) {										\
                int __i__ = 0;														\
                T*	__t_ptr__ = NULL;												\
                for(__i__=0; __i__<List_Count(__l_plist__); __i__++) {				\
                    if(!List_GetPtr(__l_plist__, __i__,								\
                                            (void**)(&__t_ptr__))) {				\
                        continue;													\
                    }																\
                                                                                    \
                    __fn_t_free__(__t_ptr__);										\
                }																	\
            }																		\
        }																			\
                                                                                    \
        List_Destroy(__l_plist__);													\
    }																				\

#else

#define List_Init_T(T, pList, bResult)												\
        (bResult) = List_Init((pList), sizeof(T));									\


#define List_Init_WithCount_T(T, pList, ExpectedCount, bResult)						\
        (bResult) = List_Init_WithCount((pList), sizeof(T), (ExpectedCount));		\


#define List_Add_T(T, pList, Data, ReturnPtr)										\
    {																				\
        T __l_data__ = (Data);														\
        (ReturnPtr) = List_Add((pList), &__l_data__);								\
    }																				\


#define List_Insert_T(T, pList, Index, Data, ReturnPtr)								\
    {																				\
        T __l_data__ = (Data);														\
        (ReturnPtr) = List_Insert((pList), (Index), &__l_data__);					\
    }																				\


#define List_PlaceHold_T(T, pList, ReturnPtr)										\
        (ReturnPtr) = List_PlaceHold((pList));										\


#define List_PlaceHoldAt_T(T, pList, Index, ReturnPtr)								\
        (ReturnPtr) = List_PlaceHoldAt((pList), (Index));							\


#define List_Remove_T(T, pList, Data, bResult)										\
        (bResult) = List_Remove((pList), &(Data));									\


#define List_Get_T(T, pList, Index, ReturnData, bResult)							\
        (bResult) = List_Get((pList), (Index), &(ReturnData));						\


#define List_GetPtr_T(T, pList, Index, ReturnPtr, bResult)							\
        (bResult) = List_GetPtr((pList), (Index), (void**)(&(ReturnPtr)));			\


#define List_Set_T(T, pList, Index, Data, bResult)									\
    {																				\
        T __l_data__ = (Data);														\
        (bResult) = List_Set((pList), (Index), &__l_data__);						\
    }																				\


#define List_Contains_T(T, pList, Data, bResult)									\
        (bResult) = List_Contains((pList), &(Data));								\


#define List_IndexOf_T(T, pList, Data, ReturnIndex)									\
        (ReturnIndex) = List_IndexOf((pList), &(Data));								\


#define List_Sort_T(T, pList, fnElementCompareFunc, bResult)						\
    {																				\
        int (*__fn_t_compare__)(T a, T b);											\
        PLIST __l_plist__ = (pList);												\
        __fn_t_compare__ = (fnElementCompareFunc);									\
                                                                                    \
        if(__l_plist__ && __fn_t_compare__ != NULL) {								\
            if(List_Count(__l_plist__) < 2) {										\
                (bResult) = TRUE;													\
            } else {																\
                int		__i__ = 0;													\
                int		__j__ = 0;													\
                int		__min__ = 0;												\
                T		__a__;														\
                T		__b__;														\
                                                                                    \
                for(__i__=0; __i__<List_Count(__l_plist__)-1; __i__++) {			\
                    __min__ = __i__;												\
                    for(__j__=__i__+1; __j__<List_Count(__l_plist__); __j__++) {	\
                        if(!List_Get(__l_plist__, __min__, &__a__)) {				\
                            continue;												\
                        }															\
                        if(!List_Get(__l_plist__, __j__, &__b__)) {					\
                            continue;												\
                        }															\
                                                                                    \
                        if(__fn_t_compare__(__b__, __a__) < 0) {					\
                            __min__ = __j__;										\
                        }															\
                    }																\
                    if(__min__ != __i__) {											\
                        List_Get(__l_plist__, __min__, &__a__);						\
                        List_Get(__l_plist__, __i__, &__b__);						\
                                                                                    \
                        List_Set(__l_plist__, __min__, &__b__);						\
                        List_Set(__l_plist__, __i__, &__a__);						\
                    }																\
                }																	\
                                                                                    \
                (bResult) = TRUE;													\
            }																		\
        } else {																	\
            (bResult) = FALSE;														\
        }																			\
    }																				\


#define List_ClearEx_T(T, pList, fnElementFreeFunc)									\
        List_ClearEx((pList), (fnElementFreeFunc));									\


#define List_RemoveAtEx_T(T, pList, Index, fnElementFreeFunc)						\
        List_RemoveAtEx((pList), (Index), (fnElementFreeFunc));						\


#define List_DestroyEx_T(T, pList, fnElementFreeFunc)								\
        List_DestroyEx((pList), (fnElementFreeFunc));								\

#endif

/*************************************************************************************************************/

/*************************************************** Queue ***************************************************/
#define QUEUE_ELEMENT_MAX_COUNT		(INT_MAX)

typedef struct _QUEUE {
    unsigned int    ElementSizeInBytes;
    int             ElementMaxCount;

    int             ElementCount;
    unsigned int    ElementHead;
    unsigned int    ElementTail;

    unsigned int    BufferSizeInBytes;
    unsigned char*  Buffer;
} QUEUE, *PQUEUE;

QUEUE Queue_Instance(IN unsigned int ElementSizeInBytes);

QUEUE Queue_Instance_WithCount(IN int ExpectedElementsCount, IN unsigned int ElementSizeInBytes);

BOOL Queue_Init(IN_OUT PQUEUE pQueue, IN unsigned int ElementSizeInBytes);

BOOL Queue_Init_WithCount(IN_OUT PQUEUE pQueue, IN unsigned int ElementSizeInBytes, IN int ExpectedElementsCount);

BOOL Queue_Set_ElementMaxCount(IN_OUT PQUEUE pQueue, int nElementMaxCount);

BOOL Queue_Push(IN_OUT PQUEUE pQueue, IN void* ElementPtr);

BOOL Queue_Pop(IN_OUT PQUEUE pQueue, OUT void* ElementPtr);

BOOL Queue_Get(IN PQUEUE pQueue, IN int idx, OUT void* ElementPtr);

BOOL Queue_GetPtr(IN PQUEUE pQueue, IN int idx, OUT void** pElementPtr);

BOOL Queue_Set(IN_OUT PQUEUE pQueue, IN int idx, IN void* ElementPtr);

BOOL Queue_Clear(IN_OUT PQUEUE pQueue);

BOOL Queue_Destroy(IN_OUT PQUEUE pQueue);

static inline int Queue_Count(IN PQUEUE pQueue)
{
    return pQueue->ElementCount;
}

#define Queue_Declare_T(T)						{sizeof(T), QUEUE_ELEMENT_MAX_COUNT, 0, 0, 0, 0, NULL, NULL}

#ifdef TYPE_CHECK

#define Queue_Init_T(T, pQueue, bResult)											\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        (bResult) = Queue_Init(__l_pqueue__, sizeof(T));							\
    }																				\


#define Queue_Init_WithCount_T(T, pQueue, ExpectedCount, bResult)					\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        unsigned int __l_expect_cnt__ = (ExpectedCount);							\
        (bResult) = Queue_Init_WithCount(__l_pqueue__, sizeof(T), __l_expect_cnt__);\
    }																				\


#define Queue_Push_T(T, pQueue, Data, bResult)										\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        T __l_data__ = (Data);														\
        (bResult) = Queue_Push(__l_pqueue__, &__l_data__);							\
    }																				\


#define Queue_Pop_T(T, pQueue, Data, bResult)										\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        T __l_data__;																\
        BOOL __l_bool__ = Queue_Pop(__l_pqueue__, &__l_data__);						\
        if(__l_bool__) {															\
            (Data) = __l_data__;													\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define Queue_Get_T(T, pQueue, Index, Data, bResult)								\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        unsigned int __l_index__ = (Index);											\
        T __l_data__;																\
        BOOL __l_bool__ = Queue_Get(__l_pqueue__, __l_index__, &__l_data__);		\
        if(__l_bool__) {															\
            (Data) = __l_data__;													\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define Queue_GetPtr_T(T, pQueue, Index, ReturnPtr, bResult)						\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        unsigned int __l_index__ = (Index);											\
        T* __l_p_data__ = NULL;														\
        BOOL __l_bool__ = Queue_GetPtr(__l_pqueue__, __l_index__,					\
                                                (void**)(&__l_p_data__));			\
        if(__l_bool__) {															\
            (ReturnPtr) = __l_p_data__;												\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define Queue_Set_T(T, pQueue, Index, Data, bResult)								\
    {																				\
        PQUEUE __l_pqueue__ = (pQueue);												\
        unsigned int __l_index__ = (Index);											\
        T __l_data__ = (Data);														\
        (bResult) = Queue_Set(__l_pqueue__, __l_index__, &__l_data__);				\
    }																				\

#else

#define Queue_Init_T(T, pQueue, bResult)											\
        (bResult) = Queue_Init((pQueue), sizeof(T));								\


#define Queue_Init_WithCount_T(T, pQueue, ExpectedCount, bResult)					\
        (bResult) = Queue_Init_WithCount((pQueue), sizeof(T), (ExpectedCount));		\


#define Queue_Push_T(T, pQueue, Data, bResult)										\
    {																				\
        T __l_data__ = (Data);														\
        (bResult) = Queue_Push((pQueue), &__l_data__);								\
    }																				\


#define Queue_Pop_T(T, pQueue, Data, bResult)										\
        (bResult) = Queue_Pop((pQueue), &(Data));									\


#define Queue_Get_T(T, pQueue, Index, Data, bResult)								\
        (bResult) = Queue_Get((pQueue), (Index), &(Data));							\


#define Queue_GetPtr_T(T, pQueue, Index, ReturnPtr, bResult)						\
        (bResult) = Queue_GetPtr((pQueue), (Index),	(void**)(&(ReturnPtr)));		\


#define Queue_Set_T(T, pQueue, Index, Data, bResult)								\
    {																				\
        T __l_data__ = (Data);														\
        (bResult) = Queue_Set((pQueue), (Index), &__l_data__);						\
    }																				\

#endif

/*************************************************************************************************************/

/************************************************* HashMap ***************************************************/
/************ Ωˆœﬁ”⁄ºÚµ•¿‡–Õ£¨ µœ÷‘≠¿ÌŒ™ƒ⁄¥Ê∏¥÷∆ *************************************************************/
/************ »Áπ˚¿‡–Õ÷–∞¸∫¨÷∏’Î£¨Ω˜…˜ π”√£¨À˘”–œ‡πÿ∑Ω∑®≤ª∏∫‘¿‡–Õƒ⁄÷∏’Îµƒƒ⁄¥Êπ‹¿Ì ***************************/
/************ »Áπ˚¿‡–Õ÷–∫¨”–À¯°¢ ¬º˛µ»”√”⁄œﬂ≥ÃÕ¨≤Ωµƒ ˝æ›£¨æ°¡ø≤ª“™ π”√∏√ ˝æ›Ω·ππ±£¥Ê *************************/

struct _HASH_MAP_NODE;
struct _HASH_MAP;

typedef struct _HASH_MAP_NODE HASH_MAP_NODE, *PHASH_MAP_NODE;
typedef struct _HASH_MAP HASH_MAP, *PHASH_MAP;

struct _HASH_MAP_NODE {
    unsigned int   Level;               //节点所在的层级
    unsigned int   bOccupied;           //节点是否被占用
    
    unsigned int    KeySizeInBytes;     //key值的字节数
    unsigned int    ValueSizeInBytes;   //value值的字节数
    unsigned char*  Key;                //存储的key值
    unsigned char*  Value;              //存储的value值

    PHASH_MAP_NODE  *NextLevelNodes;    //子节点指针

    PHASH_MAP       pChildMap;          //当到达现有树的第十层，仍然有冲突存在，则生成子HashMap
};

struct _HASH_MAP {
    unsigned int    KeySizeInBytes;         //Key类型所占字节数
    unsigned int    ValueSizeInBytes;       //Value类型所占字节数

    unsigned int*   PrimeNumberArray;       //质数序列数组
    unsigned int    PrimeNumberArrayLength; //质数序列长度

    LIST            NodePtrList;            //节点栈，在清空HashMap和销毁HashMap时使用
    PHASH_MAP_NODE  pRootNode;              //根节点指针
    long long       ItemCount;              //存储的元素数量
    long long       TotalNodeCount;         //总节点数
};

struct _HASH_MAP_ITERATOR;

typedef struct _HASH_MAP_ITERATOR HASH_MAP_ITERATOR, *PHASH_MAP_ITERATOR;

struct _HASH_MAP_ITERATOR {
    PHASH_MAP           pMap;           //指向需要遍历的HashMap

    PHASH_MAP_ITERATOR  pChildIterator; //子迭代器
    LIST                NodePtrList;    //节点指针栈
    long long           Counter;        //已经遍历的节点数量
};

BOOL HashMap_Init(IN_OUT PHASH_MAP pHashMap, IN unsigned int KeySizeInBytes, IN unsigned int ValueSizeInBytes,
                OPTIONAL_IN unsigned int PrimeNumberArray[], OPTIONAL_IN unsigned int PrimeNumberArrayLength);

void* HashMap_Add(IN_OUT PHASH_MAP pHashMap, IN void* pKey, IN void* pValue);

void* HashMap_PlaceHold(IN_OUT PHASH_MAP pHashMap, IN void* pKey);

BOOL HashMap_Get(IN PHASH_MAP pHashMap, IN void* pKey, OUT void* pValue);

BOOL HashMap_GetPtr(IN PHASH_MAP pHashMap, IN void* pKey, OUT void** pValuePtr);

BOOL HashMap_Set(IN_OUT PHASH_MAP pHashMap, IN void* pKey, IN void* pValue);

BOOL HashMap_Remove(IN_OUT PHASH_MAP pHashMap, IN void* pKey);

BOOL HashMap_RemoveEx(IN_OUT PHASH_MAP pHashMap, IN void* pKey,
                    void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue));

BOOL HashMap_ContainsKey(IN PHASH_MAP pHashMap, IN void* pKey);

BOOL HashMap_Clear(IN_OUT PHASH_MAP pHashMap);

BOOL HashMap_ClearEx(IN_OUT PHASH_MAP pHashMap,
                    void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue));

void HashMap_Destroy(IN_OUT PHASH_MAP pHashMap);

void HashMap_DestroyEx(IN_OUT PHASH_MAP pHashMap,
                    void (*fnKeyFreeFunc)(void* pKey), void (*fnValueFreeFunc)(void* pValue));

static inline long long HashMap_Count(IN PHASH_MAP pHashMap)
{
    return pHashMap->ItemCount;
}

BOOL HashMap_Iterator_Init(IN PHASH_MAP_ITERATOR pIterator, IN PHASH_MAP pHashMap);

BOOL HashMap_Iterator_Reset(IN PHASH_MAP_ITERATOR pIterator);

BOOL HashMap_Iterator_GetNext(IN PHASH_MAP_ITERATOR pIterator, OUT void* pKey, OUT void* pValue);

BOOL HashMap_Iterator_GetNextPtr(IN PHASH_MAP_ITERATOR pIterator, OUT void** pKeyPtr, OUT void** pValuePtr);

BOOL HashMap_Iterator_Destroy(IN PHASH_MAP_ITERATOR pIterator);

// #define HashMap_Declarer_T(TKey, TValue)	({sizeof(TKey), sizeof(TValue), 0, 0, 0, 0})

#ifdef TYPE_CHECK

#define HashMap_Init_Default_T(TKey, TValue, pHashMap, bResult)						\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        (bResult) = HashMap_Init(__l_pmap__, sizeof(TKey), sizeof(TValue),			\
                                                                NULL, 0);           \
    }																				\


#define HashMap_Add_T(TKey, TValue, pHashMap, Key, Value, ReturnPtr)				\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        TValue __l_value__ = (Value);												\
        (ReturnPtr) = HashMap_Add(__l_pmap__, &__l_key__, &__l_value__);			\
    }																				\


#define HashMap_PlaceHold_T(TKey, TValue, pHashMap, Key, ReturnPtr)					\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        TValue* __l_p_value__ = HashMap_PlaceHold(__l_pmap__, &__l_key__);			\
        (ReturnPtr) = __l_p_value__;												\
    }																				\


#define HashMap_Get_T(TKey, TValue, pHashMap, Key, ReturnValue, bResult)			\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        TValue __l_value__;															\
        BOOL __l_bool__ = HashMap_Get(__l_pmap__, &__l_key__, &__l_value__);		\
        if(__l_bool__) {															\
            (ReturnValue) = __l_value__;											\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define HashMap_GetPtr_T(TKey, TValue, pHashMap, Key, ReturnPtr, bResult)			\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        TValue* __l_p_value__ = NULL;												\
        BOOL __l_bool__ = HashMap_GetPtr(__l_pmap__, &__l_key__,					\
                                                (void**)(&__l_p_value__));			\
        if(__l_bool__) {															\
            (ReturnPtr) = __l_p_value__;											\
        }																			\
        (bResult) = __l_bool__;														\
    }																				\


#define HashMap_Set_T(TKey, TValue, pHashMap, Key, Value, bResult)					\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        TValue __l_value__ = (Value);												\
        (bResult) = HashMap_Set(__l_pmap__, &__l_key__, &__l_value__);				\
    }																				\


#define HashMap_Remove_T(TKey, pHashMap, Key, bResult)								\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_Remove(__l_pmap__, &__l_key__);							\
    }																				\


#define HashMap_RemoveEx_T(TKey, TValue, pHashMap, Key, fnKeyFreeFunc, fnValueFreeFunc, bResult)	\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        void (*__fn_key_free__)(TKey* pKey);										\
        void (*__fn_value_free__)(TValue* pValue);									\
                                                                                    \
        __fn_key_free__ = (fnKeyFreeFunc);											\
        __fn_value_free__ = (fnValueFreeFunc);										\
                                                                                    \
        (bResult) = HashMap_RemoveEx(__l_pmap__, &__l_key__,						\
            __fn_key_free__, __fn_value_free__);									\
    }																				\


#define HashMap_ContainsKey_T(TKey, pHashMap, Key, bResult)							\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_ContainsKey(__l_pmap__, &__l_key__);					\
    }																				\

#define HashMap_Clear_T(TKey, TValue, pHashMap, bResult)							\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        (bResult) = HashMap_Clear(__l_pmap__);										\
    }																				\


#define HashMap_ClearEx_T(TKey, TValue, pHashMap, fnKeyFreeFunc, fnValueFreeFunc, bResult)	\
    {																				\
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        void (*__fn_key_free__)(TKey* pKey);										\
        void (*__fn_value_free__)(TValue* pValue);									\
                                                                                    \
        __fn_key_free__ = (fnKeyFreeFunc);											\
        __fn_value_free__ = (fnValueFreeFunc);										\
                                                                                    \
        (bResult) = HashMap_ClearEx(__l_pmap__, __fn_key_free__, __fn_value_free__);\
    }																				\


#define HashMap_DestroyEx_T(TKey, TValue, pHashMap, fnKeyFreeFunc, fnValueFreeFunc)	\
    {																				\
        void (*__fn_key_free__)(TKey* pKey);										\
        void (*__fn_value_free__)(TValue* pValue);									\
                                                                                    \
        PHASH_MAP __l_pmap__ = (pHashMap);											\
        HASH_MAP_ITERATOR __l_iterator__ = {0};										\
                                                                                    \
        TKey*	__l_p_key__ = NULL;													\
        TValue* __l_p_value__ = NULL;												\
                                                                                    \
        __fn_key_free__ = (fnKeyFreeFunc);											\
        __fn_value_free__ = (fnValueFreeFunc);										\
                                                                                    \
        if(__l_pmap__ && (__fn_key_free__ != NULL || __fn_value_free__ != NULL)) {	\
            if(HashMap_Iterator_Init(&__l_iterator__, __l_pmap__)) {				\
                while(HashMap_Iterator_GetNextPtr(&__l_iterator__,					\
                            (void**)(&__l_p_key__), (void**)(&__l_p_value__))) {	\
                    if(__fn_key_free__ != NULL) {									\
                        __fn_key_free__(__l_p_key__);								\
                    }																\
                                                                                    \
                    if(__fn_value_free__ != NULL) {									\
                        __fn_value_free__(__l_p_value__);							\
                    }																\
                }																	\
                HashMap_Iterator_Destroy(&__l_iterator__);							\
            }																		\
        }																			\
                                                                                    \
        HashMap_Destroy(__l_pmap__);												\
    }																				\


#define HashMap_Iterator_GetNext_T(TKey, TValue, pIterator, Key, Value, bResult)				\
    {																							\
        PHASH_MAP_ITERATOR __l_piterator__ = (pIterator);										\
        TKey* __l_p_key__ = &(Key);																\
        TValue* __l_p_value__ = &(Value);														\
        (bResult) = HashMap_Iterator_GetNext(__l_piterator__, __l_p_key__, __l_p_value__);		\
    }																							\


#define HashMap_Iterator_GetNextPtr_T(TKey, TValue, pIterator, pKey, pValue, bResult)			\
    {																							\
        PHASH_MAP_ITERATOR __l_piterator__ = (pIterator);										\
        TKey** __l_pp_key__ = &(pKey);															\
        TValue** __l_pp_value__ = &(pValue);													\
        (bResult) = HashMap_Iterator_GetNextPtr(__l_piterator__,								\
                            (void**)__l_pp_key__,  (void**)__l_pp_value__);						\
    }																							\

#else

#define HashMap_Init_Default_T(TKey, TValue, pHashMap, bResult)						\
        (bResult) = HashMap_Init((pHashMap), sizeof(TKey), sizeof(TValue),			\
                                                                NULL, 0);           \


#define HashMap_Add_T(TKey, TValue, pHashMap, Key, Value, ReturnPtr)				\
    {																				\
        TKey __l_key__ = (Key);														\
        TValue __l_value__ = (Value);												\
        (ReturnPtr) = HashMap_Add((pHashMap), &__l_key__, &__l_value__);			\
    }																				\


#define HashMap_PlaceHold_T(TKey, TValue, pHashMap, Key, ReturnPtr)					\
    {																				\
        TKey __l_key__ = (Key);														\
        (ReturnPtr) = HashMap_PlaceHold((pHashMap), &__l_key__);					\
    }																				\


#define HashMap_Get_T(TKey, TValue, pHashMap, Key, ReturnValue, bResult)			\
    {																				\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_Get((pHashMap), &__l_key__, &(ReturnValue));			\
    }																				\


#define HashMap_GetPtr_T(TKey, TValue, pHashMap, Key, ReturnPtr, bResult)			\
    {																				\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_GetPtr((pHashMap), &__l_key__,							\
                                                (void**)(&(ReturnPtr)));			\
    }																				\


#define HashMap_Set_T(TKey, TValue, pHashMap, Key, Value, bResult)					\
    {																				\
        TKey __l_key__ = (Key);														\
        TValue __l_value__ = (Value);												\
        (bResult) = HashMap_Set((pHashMap), &__l_key__, &__l_value__);				\
    }																				\


#define HashMap_Remove_T(TKey, pHashMap, Key, bResult)								\
    {																				\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_Remove((pHashMap), &__l_key__);							\
    }																				\


#define HashMap_RemoveEx_T(TKey, TValue, pHashMap, Key, fnKeyFreeFunc, fnValueFreeFunc, bResult)	\
        (bResult) = HashMap_RemoveEx((pHashMap), &(Key),							\
            (fnKeyFreeFunc), (fnValueFreeFunc));									\


#define HashMap_ContainsKey_T(TKey, pHashMap, Key, bResult)							\
    {																				\
        TKey __l_key__ = (Key);														\
        (bResult) = HashMap_ContainsKey((pHashMap), &__l_key__);					\
    }																				\

#define HashMap_Clear_T(TKey, TValue, pHashMap, bResult)							\
        (bResult) = HashMap_Clear((pHashMap));										\


#define HashMap_ClearEx_T(TKey, TValue, pHashMap, fnKeyFreeFunc, fnValueFreeFunc, bResult)	\
        (bResult) = HashMap_ClearEx((pHashMap), (fnKeyFreeFunc), (fnValueFreeFunc));\


#define HashMap_DestroyEx_T(TKey, TValue, pHashMap, fnKeyFreeFunc, fnValueFreeFunc)	\
        HashMap_DestroyEx((pHashMap), (fnKeyFreeFunc), (fnValueFreeFunc));			\


#define HashMap_Iterator_GetNext_T(TKey, TValue, pIterator, Key, Value, bResult)	\
        (bResult) = HashMap_Iterator_GetNext((pIterator), &(Key), &(Value));		\


#define HashMap_Iterator_GetNextPtr_T(TKey, TValue, pIterator, pKey, pValue, bResult)	\
        (bResult) = HashMap_Iterator_GetNextPtr((pIterator),							\
                            (void**)(&(pKey)),  (void**)(&(pValue)));					\

#endif

/**************************************************************************************************************/

#endif
