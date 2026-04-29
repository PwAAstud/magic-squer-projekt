#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>

#define swap(a,b) typeof(a) t = a; a = b; b = t;

bool nextPerm(bool* list, uint32_t size){
    uint32_t pivot = 1;
    // while(pivot < size && list[pivot-1] <= list[pivot]) pivot++;
    // 00 01 11 10
    while(pivot < size && !(list[pivot-1] && !list[pivot])) pivot++;
    uint32_t toSwap = 0;
    if(pivot < size){
        // while(list[toSwap] <= list[pivot]) toSwap++;
        while(!list[toSwap]) toSwap++;
        swap(list[pivot], list[toSwap]);
        // for(uint32_t i = 0; i < size; i++) printf("%d ", list[i]); printf("\n");
        // printf("pivot %d swap %d\n", pivot, toSwap);
    }
    uint32_t a = 0;
    uint32_t b = pivot-1;
    while (a < b){
        swap(list[a], list[b]);
        a++; b--;
    }
    return !(pivot < size);
}

uint32_t sumPerm(bool* list, uint32_t size){
    uint32_t sum = 0;
    for(uint32_t i = 0; i < size; i++){
        if(!list[i]) continue;
        sum += 1 + i;
    }
    return sum;
}

int main(int argc, const char* argv[]){
    const uint32_t sideSize = (argc > 1) ? atoi(argv[1]) : 3;
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    bool ocuapy[size];
    for(uint32_t i = 0; i < size; i++) ocuapy[i] = false;
    for(uint32_t i = 0; i < sideSize; i++) ocuapy[i] = true;
    uint32_t dgtCound[size];
    for(uint32_t i = 0; i < size; i++) dgtCound[i] = 0;

    uint32_t goodNum = 0;
    uint32_t all = 0;
    for(;;){
        // for(uint32_t i = 0; i < size; i++) printf("%d ", ocuapy[i]); printf("\n");
        uint32_t sum = sumPerm(ocuapy, size);
        // printf("sum %d\n", sum);
        all++;
        if(sum == goodSum){
            for(uint32_t i = 0; i < size; i++){
                if(!ocuapy[i]) continue;
                // printf("%2d ", i+1);
                dgtCound[i]++;
            }
            // printf("\n");
            goodNum++;
        }
        if(nextPerm(ocuapy, size)) break;
    }

    for(uint32_t i = 0; i < size; i++) printf("%3d ", i+1); printf("\n");
    for(uint32_t i = 0; i < size; i++) printf("%3d ", dgtCound[i]); printf("\n");
    printf("good num %3d\n", goodNum);
    printf("stosunek %f\n", (float)goodNum/all);
    // printf("min num  %3d\n", 2*sideSize+2);
    
}