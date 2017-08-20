//
//  main.c
//  DataStructure
//
//  Created by Uiworks on 2017/8/16.
//  Copyright © 2017年 Uiworks. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include "tools.h"

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void test(PHASH_MAP map, int count) {
    long start = 0;
    long end = 0;
    
    int *randNumbers = calloc(count, sizeof(int));
    for(int i=0; i<count; i++) {
        randNumbers[i] = rand();
    }
    
    start = getCurrentTime();
    for(int i=0; i<count; i++) {
        int key = randNumbers[i];
        int value = key;
        HashMap_Add(map, &key, &value);
    }
    end = getCurrentTime();
    printf("插入%d个随机数 : %ld\n", count, end - start);
    
    start = getCurrentTime();
    for (int i=0; i<count; i++) {
        int key = randNumbers[i];
        int value = 0;
        HashMap_Get(map, &key, &value);
        if(key != value) {
            printf("查询失败\n");
        }
    }
    end = getCurrentTime();
    printf("查找%d个随机数 : %ld\n", count, end - start);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
//    HASH_MAP map;
//    BOOL bResult;
//    HashMap_Init_Default_T(int, int, &map, bResult);
//    
//    srand((unsigned int)time(NULL));
//    for(int i=0; i<10000000; i++) {
//        int key = rand();
//        int value = key;
//        HashMap_Add(&map, &key, &value);
//    }
//    printf("%ld\n", map.TotalNodeCount);
//    getchar();
    
    long start = 0;
    long end = 0;
    
    HASH_MAP map;
    BOOL bResult;
    HashMap_Init_Default_T(int, int, &map, bResult);
    
    srand((unsigned int)time(NULL));
    
    start = getCurrentTime();
    for(int i=0; i<10000000; i++) {
        int key = rand();
        int value = key;
        HashMap_Add(&map, &key, &value);
    }
    end = getCurrentTime();
    printf("生成10000000随机数的HashMap : %ld\n", end - start);
    
//    test(&map, 1);
//    test(&map, 10);
//    test(&map, 100);
//    test(&map, 1000);
//    test(&map, 10000);
    
    start = getCurrentTime();
    HashMap_Clear(&map);
    end = getCurrentTime();
    printf("清空10000000随机数HashMap : %ld\n", end - start);
    
    start = getCurrentTime();
    HashMap_Destroy(&map);
    end = getCurrentTime();
    printf("销毁10000000随机数HashMap : %ld\n", end - start);
    
//    getchar();
    
//    int *numbers = malloc(10000000 * 4);
//    start = getCurrentTime();
//    for(int i=0; i<10000000; i++) {
//        int value = rand();
//        numbers[i] = value;
//        
//        if(i%100000 == 0) {
//            randNumbers[i/100000] = value;
//        }
//    }
//    end = getCurrentTime();
//    printf("time4 : %ld\n", end - start);
//    
//    start = getCurrentTime();
//    for(int i=0; i<100; i++) {
//        for(int j=0; j<10000000; j++) {
//            if(numbers[j] == randNumbers[i]) {
//                printf("%d : %d\n", randNumbers[i], numbers[j]);
//                break;
//            }
//        }
//    }
//    end = getCurrentTime();
//    printf("time5 : %ld\n", end - start);
//    
//    getchar();
//    
//    free(numbers);
    
    return 0;
}
