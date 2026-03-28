#include<stdio.h>
#include"lexicalPerm.c"

void printSquer(squerVal* squer, uint32_t sideSize){
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%2d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    printf("\n");
}

void t1(){
    // const uint32_t sideSize = 2;
    const uint32_t size = 3;
    squerVal squer[size];
    for(uint32_t i = 0; i < size; i++) squer[i] = i+1;

    for(uint32_t x = 0; x < size; x++) printf("%2d", squer[x]);
    printf("\n");
    for(uint32_t i = 0; i < 8; i++){
        // printSquer(squer, sideSize);
        nextPermutation(squer, size);
        for(uint32_t x = 0; x < size; x++) printf("%2d", squer[x]);
        printf("\n");
    }
}

int main(){
    const uint32_t sideSize = 4;
    const uint32_t size = sideSize*sideSize;
    squerVal squer[size];
    generateMagicSquer_lexical(squer, sideSize);
    printf("\n");
    printSquer(squer, sideSize);
}