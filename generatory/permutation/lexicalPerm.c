#include"lexicalPerm.h"
#include<stdbool.h>
#include<stdio.h>

// https://www.nayuki.io/page/next-lexicographical-permutation-algorithm

static bool isMagical(squerVal* squer, uint32_t sideSize){
    uint32_t lastSum = 0;
    for(uint32_t x = 0; x < sideSize; x++) lastSum += squer[x];
    uint32_t curentSum;
    // row
    for(uint32_t y = 1; y < sideSize; y++){
        curentSum = 0;
        for(uint32_t x = 0; x < sideSize; x++) curentSum += squer[y*sideSize + x];
        if(lastSum != curentSum) return false;
    }
    // col
    for(uint32_t x = 1; x < sideSize; x++){
        curentSum = 0;
        for(uint32_t y = 0; y < sideSize; y++) curentSum += squer[y*sideSize + x];
        if(lastSum != curentSum) return false;
    }

    // cross
    curentSum = 0;
    for(uint32_t i = 0; i < sideSize; i++) curentSum += squer[i*sideSize + i];
    if(lastSum != curentSum) return false;
    curentSum = 0;
    for(uint32_t i = 0; i < sideSize; i++) curentSum += squer[i*sideSize + sideSize - 1 - i];
    if(lastSum != curentSum) return false;

    return true;
}

#define swap(a,b) typeof(a) t = a; a = b; b = t

static void nextPermutation(squerVal* squer, uint32_t size){
    // printf("%d\n", size);

    uint32_t pivot = size-2;
    for(; 0 < pivot && squer[pivot] >= squer[pivot + 1] ; pivot--);
    uint32_t toSwap = size-1;
    for(; squer[pivot] > squer[toSwap] ; toSwap--);
    // printf("%d %d\n", pivot, toSwap);
    swap(squer[pivot], squer[toSwap]);
    
    // rotate
    uint32_t a = pivot + 1 - (pivot == toSwap); 
    uint32_t b = size - 1;
    while (a < b){
        swap(squer[a], squer[b]);
        a++; b--;
    }
}

void generateMagicSquer_lexical(squerVal* out, uint32_t sideSize){
    const uint32_t size = sideSize*sideSize;
    for(uint32_t i = 0; i < sideSize*sideSize; i++) out[i] = i+1;
    uint32_t i = 0;
    // printf("\033[?25lj");
    while (!isMagical(out, sideSize)){
        // printf("\r%4d", i++);
        nextPermutation(out, size);
    }
    // printf("\033[?25h");
    // printf("\n");
}