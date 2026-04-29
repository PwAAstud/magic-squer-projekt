#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>

#define swap(a,b) typeof(a) t = a; a = b; b = t;

bool nextPerm(bool* list, uint32_t size){
    uint32_t pivot = 1;
    // 00 01 11 10
    while(pivot < size && !(list[pivot-1] && !list[pivot])) pivot++;
    uint32_t toSwap = 0;
    if(pivot < size){
        while(!list[toSwap]) toSwap++;
        swap(list[pivot], list[toSwap]);
    }
    uint32_t a = 0;
    uint32_t b = pivot-1;
    while (a < b){
        swap(list[a], list[b]);
        a++; b--;
    }
    return !(pivot < size);
}

bool nextGoodPerm(bool* list, uint32_t size, int goodSum){
    uint32_t sum;
    do{
        // printf("%d\n", nextPerm(list, size));
        if(nextPerm(list, size)) return true;
        sum = 0;
        for(uint32_t i = 0; i < size; i++){
            if(!list[i]) continue;
            sum += 1 + i;
        }    
    }while(sum != goodSum);
    return false;
}

int main(int argc, const char* argv[]){
    const uint32_t sideSize = (argc > 1) ? atoi(argv[1]) : 3;
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;
    printf("good sum %d\n", goodSum);

    bool ocuapy[size];
    for(uint32_t i = 0; i < size; i++) ocuapy[i] = false;
    for(uint32_t i = 0; i < sideSize; i++) ocuapy[i] = true;

    // contex; prob; contex naraze tylko 1
    uint32_t prob[size][size];
    for(uint32_t i=0;i<size;i++){
        for(uint32_t j=0;j<size;j++){
            // printf("%d %d\n", i,j);
            prob[i][j] = 0;
        }
    }

    for(;;){
        if(nextGoodPerm(ocuapy, size, goodSum)) break;

        // wyswietl wersje
        // if(ocuapy[0] == true && ocuapy[size-1] == true){
            for(int i=0; i < size; i++){
    
                if(ocuapy[i] == 0) continue;
                printf("%2d ", i+1);
            }
            printf("\n");
        // }

        // zlicz w kontekscie
        for(uint32_t contex=0; contex<size; contex++){
            if(ocuapy[contex] == 0) continue;
            for(uint32_t i=0;i<size;i++) prob[contex][i] += (ocuapy[i] == true);
        }
    }

    for(uint32_t contex=0; contex<size; contex++){
        printf("constex %d\n", contex+1);
        prob[contex][contex] = 0;
        for(uint32_t i = 0; i < size; i++) printf("%3d ", i+1); printf("\n");
        for(uint32_t i = 0; i < size; i++) printf("%3d ", prob[contex][i]); printf("\n");
    }

    return 0;
}