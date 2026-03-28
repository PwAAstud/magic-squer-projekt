#include<stdio.h>
#include"force.c"

static void printSquerA(squerVal* squer, uint32_t sideSize){
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%3d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    printf("\n");
}

int main(){
    const uint32_t sideSize = 5;
    const uint32_t size = sideSize*sideSize;
    squerVal squer[size];
    generateMagicSquer_force(squer, sideSize);
    printSquerA(squer, sideSize);
}