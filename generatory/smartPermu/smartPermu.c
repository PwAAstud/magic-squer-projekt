#include"smartPermu.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define swap(a,b) typeof(a) t = a; a = b; b = t
#define goodSum(sideSize) const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

static bool nextPermutation(uint32_t* list, uint32_t size, uint32_t startPivot){
    // start to jaka dzies ma byc odana do premutacji
    // printf("%d\n", size);
    uint32_t sortFrom = startPivot + 1;
    for(uint32_t jump = (size - sortFrom)/2; jump > 0; jump /= 2){
        for(uint32_t a = sortFrom + jump; a < size; a++){
            for(;a >= sortFrom + jump; a -= jump){
                uint32_t b = a-jump;
                if(!(list[b] < list[a])) break;
                swap(list[b],list[a]);
            }
        }
    }

    // int32_t pivot = (size != start) ? start-1 : size - 2;
    int32_t pivot = (size > sortFrom) ? startPivot : size - 2;
    for(; 0 <= pivot && list[pivot] >= list[pivot + 1]; pivot--);
    if(0 <= pivot){
        int32_t toSwap = size-1;
        for(; list[pivot] >= list[toSwap]; toSwap--);
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

static int32_t whereNotMagical(squerVal* squer, uint32_t sideSize){
    goodSum(sideSize);
    uint32_t sum = 0;

    // rows
    for (uint32_t i = 0; i < sideSize-1; i++){
        sum = 0;
        for(uint32_t j = i*sideSize; j < (i+1)*sideSize; j++) sum += squer[j];
        if(sum != goodSum) return (i+1)*sideSize - 1;
    }

    // right
    sum = 0;
    for (uint32_t i = 0; i < sideSize; i++) sum += squer[sideSize*i + sideSize - i - 1];
    if(sum != goodSum) return (sideSize-1)*sideSize;
    
    // colm
    for (uint32_t i = 0; i < sideSize-1; i++){
        sum = 0;
        for(uint32_t j = i; j < sideSize*sideSize; j+=sideSize) sum += squer[j];
        if(sum != goodSum) return (sideSize-1)*sideSize + i;
    }

    // last row
    sum = 0;
    for(uint32_t j = (sideSize-1)*sideSize; j < sideSize*sideSize; j++) sum += squer[j];
    if(sum != goodSum) return sideSize*sideSize - 1;

    // left
    sum = 0;
    for (uint32_t i = 0; i < sideSize; i++) sum += squer[sideSize*i + i];
    if(sum != goodSum) return sideSize*sideSize - 1;

    return -1;
}

void generateMagicSquer_smartPermu(squerVal* out, uint32_t sideSize){
    const uint32_t size = sideSize*sideSize;
    for(uint32_t i = 0; i < size; i++) out[i] = i+1;

    // printf("\x1B[?25l");
    // for(uint32_t i = 0; i < size; i++) printf("%2d ", out[i]); printf("\r");
    while(true){
        int32_t whereFix = whereNotMagical(out, sideSize);
        if(whereFix == -1) break;
        nextPermutation(out, size, whereFix);
        // for(uint32_t i = 0; i < size; i++) printf("%2d ", out[i]); printf("\r");
    }
    // printf("\n");
    // printf("\x1B[?25h");
}