
#ifndef UI_MEMORY_H_
#define UI_MEMORY_H_

#include "platform.h"

/**************************** Platform Sensitive Functions ********************************/

#define AllocateMemory(SizeInBytes)     calloc(SizeInBytes, sizeof(char))
//#define AllocateMemory(SizeInBytes)     malloc(SizeInBytes)
#define FreeMemory(Ptr)                 free(Ptr)

/********************************************************************************************/

#endif //UI_MEMORY_H_
