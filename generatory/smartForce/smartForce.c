#include"smartForce.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define goodSum(sideSize) const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

static bool nextPermutation(uint32_t* list, uint32_t size){
    // printf("%d\n", size);

    int32_t pivot = size-2;
    for(; 0 <= pivot && list[pivot] >= list[pivot + 1] ; pivot--);
    if(0 <= pivot){
        int32_t toSwap = size-1;
        for(; list[pivot] >= list[toSwap] ; toSwap--);
        swap(list[pivot], list[toSwap]);
    }
    
    // rotate
    int32_t a = pivot + 1; 
    int32_t b = size - 1;
    while (a < b){
        swap(list[a], list[b]);
        a++; b--;
    }
    return !(0 <= pivot);
}

static void generateRow(squerVal* squer, uint32_t sideSize){
    goodSum(sideSize);
    const uint32_t size = sideSize*sideSize;

    for(uint32_t i = 0; i < size; i++) squer[i] = 0;

    // +2 poniewasz pierwszy jest nie urzywany a ostanie jest aby zatrzymac petle
    bool ocupade[size+2];
    for(uint32_t i = 0; i < size+2; i++) ocupade[i] = false;

    // printf("good sum: %d\n\n", goodSum);
    uint32_t sums[sideSize];
    for(uint32_t i = 0; i < sideSize; i++) sums[i] = goodSum;

    for(uint32_t i = 0; i < size;){
        // for(uint32_t i=0; i < sideSize; i++) printf("%d ", sums[i]);
        // printf("\n");
        // for(uint32_t i=0; i < size; i++) printf("%d ", squer[i]);
        // printf("\n\n");
        
        uint32_t x = i%sideSize, y = i/sideSize; 
        squerVal newNum;
        if(squer[i] == 0 && x == sideSize-1 ){
            newNum = sums[y];
            if(newNum == 0 || ocupade[newNum]){
                i--;
                continue;
            }
        }else{
            newNum = squer[i] + 1;
            while(ocupade[newNum]) newNum++;
        }

        ocupade[squer[i]] = false;
        sums[y] += squer[i];
        squer[i] = 0;

        if(newNum > size){
            i--;
        }else{
            ocupade[newNum] = true;
            squer[i]    = newNum;
            sums[y]    -= newNum;
            i++;
        }
    }
    for(uint32_t i=0; i < size; i++) printf("%d ", squer[i]);
    printf("\n");
}

static void generateColm(squerVal* squer, uint32_t sideSize){
    goodSum(sideSize);
    const uint32_t size = sideSize*sideSize;

    bool ocupade[size+1];
    for(uint32_t i = 0; i < size+1; i++) ocupade[i] = false;

    uint32_t sums[sideSize];
    for(uint32_t i = 0; i < sideSize; i++) sums[i] = goodSum;

    const uint32_t nil = size;
    uint32_t rowOrder[size];
    for(uint32_t i = 0; i < size; i++) rowOrder[i] = nil;

    for(uint32_t i = 0; i < size;){
        // for(uint32_t i=0; i < sideSize; i++) printf("%d ", sums[i]);
        // printf("\n");
        // for(uint32_t i=0; i < size; i++) printf("%d ", rowOrder[i]);
        // printf("\n");

        uint32_t x = i%sideSize, y = i/sideSize;
        uint32_t order_i = x*sideSize + y;

        uint32_t newNum = rowOrder[order_i];
        if(newNum != nil){
            sums[y] += squer[newNum];
            ocupade[newNum] = false;
            newNum++;
        }
        else newNum = x*sideSize;
        while(ocupade[newNum]) newNum++;
        // printf("order_i %d\n", order_i);
        // printf("new num %d\n", newNum);
        // printf("squer[newNum] %d\n", squer[newNum]);

        if(newNum < (x+1)*sideSize && squer[newNum] <= sums[y]){
            sums[y] -= squer[newNum];
            rowOrder[order_i] = newNum;
            ocupade[newNum] = true;
            i++;
        }else{
            // printf("a\n");
            rowOrder[order_i] = nil;
            i--;
        }
        // printf("\n");
    }
    // for(uint32_t i=0; i < size; i++) printf("%d ", rowOrder[i]);
    // printf("\n");

    squerVal tempSquer[size];
    for(uint32_t i=0; i < size; i++) tempSquer[i] = squer[i];
    for(uint32_t i=0; i < size; i++) squer[i] = tempSquer[rowOrder[i]];

    for(uint32_t i=0; i < size; i++) printf("%d ", squer[i]);
    printf("\n");
}

static void generateLeft(squerVal* squer, uint32_t sideSize){
    goodSum(sideSize);
    const uint32_t size = sideSize*sideSize;

    bool ocupade[sideSize+1];
    for(uint32_t i = 0; i < size+1; i++) ocupade[i] = false;

    uint32_t sum = goodSum;

    const uint32_t nil = sideSize;
    uint32_t colmOrder[size];
    for(uint32_t i = 0; i < size; i++) colmOrder[i] = nil;

    for(uint32_t y = 0; y < sideSize;){
        printf("y %d\n", y);
        for(uint32_t i = 0; i < sideSize; i++) printf("%2d ", colmOrder[i]); printf("\n");
        for(uint32_t i = 0; i < y; i++) printf("%2d ", squer[i*sideSize + colmOrder[i]]); printf("\n");
        printf("sum %d\n", sum);

        uint32_t newNum = colmOrder[y];
        if(newNum != nil){
            sum += squer[y*sideSize + newNum];
            ocupade[newNum] = false;
            newNum++;
        }
        else newNum = 0;
        while(ocupade[newNum]) newNum++;
        printf("newNum %d\n", newNum);

        uint32_t squer_i = y*sideSize + newNum;
        printf("squer[squer_i] %d\n", squer[squer_i]);
        if(newNum < sideSize
            && (y != sideSize-1 && squer[squer_i] < sum || y == sideSize-1 && squer[squer_i] == sum)
        ){
            sum -= squer[squer_i];
            colmOrder[y] = newNum;
            ocupade[newNum] = true;
            y++;
        }else{
            colmOrder[y] = nil;
            y--;
        
        }
        printf("\n");
    }
    for(uint32_t i = 0; i < sideSize; i++) printf("%d ", colmOrder[i]); printf("\n");
}

void generateMagicSquer_smartForce(squerVal* out, uint32_t sideSize){
    generateRow(out, sideSize);
    generateColm(out, sideSize);
    generateLeft(out, sideSize);
}