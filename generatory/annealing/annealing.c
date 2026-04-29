#include"annealing.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define dif(a, b) ((a >= b) ? a-b : b-a)

static void shufle(squerVal* list, uint32_t size){
    for(uint32_t i = size; i-- > 1;){
        uint32_t toSwap = rand()%i;
        swap(list[i], list[toSwap]);
    }
}

static uint32_t squerCost(squerVal* list, uint32_t sideSize, uint32_t goodSum){
    uint32_t retVal = 0;
    uint32_t sum = 0;
    // row
    for(uint32_t y = 0; y < sideSize; y++){
        sum = 0;
        for(uint32_t x = 0; x < sideSize; x++) sum += list[y*sideSize+x];
        retVal += dif(sum, goodSum);
    }

    // colm
    for(uint32_t x = 0; x < sideSize; x++){
        sum = 0;
        for(uint32_t y = 0; y < sideSize; y++) sum += list[y*sideSize+x];
        retVal += dif(sum, goodSum);
    }

    // left
    sum = 0;
    for(uint32_t i = 0; i < sideSize; i++) sum += list[i*sideSize+i];
    retVal += dif(sum, goodSum);

    // right
    sum = 0;
    for(uint32_t i = 0; i < sideSize; i++) sum += list[i*sideSize + (sideSize-1-i)];
    retVal += dif(sum, goodSum);

    return retVal;
}

static void printSquer(squerVal* squer, uint32_t sideSize){
    printf("\x1B[%dA", sideSize);
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%4d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    // printf("\n");
}

void generateMagicSquer_annealing(squerVal* out, uint32_t sideSize){
    // printf("%d\n", dif(1,3));
    // printf("%d\n", dif(3,1));
    // return 0;

    srand(time(NULL));
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    for(uint32_t i=0;i<size;i++) out[i] = i+1;
    shufle(out, size);

    const int randRange = size*size;
    int temperature = randRange/2;
    // int minTemper = ((randRange + randRange<<1) >> 8) + 1; // = randRange*3/256 + 1
    int minTemper = 1;
    int coling = 0;

    squerVal futureSquer[size];
    for(uint32_t i=0;i<size;i++) futureSquer[i] = out[i];
    uint32_t curentSquer = squerCost(out, sideSize, goodSum);
    // for(uint32_t i=0;i<sideSize;i++) printf("\n");
    // for(int trySwap = 0; trySwap < 999999; trySwap++){
    for(;;){
        // printSquer(out, sideSize);
        // printf("temperature: %10d\n", temperature);
        // printf("const      : %10d\n", curentSquer);
        // printf("\x1B[2A");
        // break;

        uint32_t swapA = rand()%size;
        uint32_t swapB = rand()%size;
        swap(futureSquer[swapA],futureSquer[swapB]);
        
        uint32_t futureCost = squerCost(futureSquer, sideSize, goodSum);
        
        // futureCost < curentSquer || !(futureCost < curentSquer) && rand
        // (a || b)
        // curentSquer < futureCost
        int goBad = rand()%randRange < (int)temperature;
        // printf("%d ", goBad);
        if(futureCost < curentSquer || goBad){
            // trySwap = 0;
            curentSquer = futureCost;
            swap(out[swapA],out[swapB]);
        }
        else{
            swap(futureSquer[swapA],futureSquer[swapB]);
        }
        // temperature = temperature*coling + (1-coling)*minTemper;
        // temperature = coling*(temperature - minTemper) + minTemper;
        coling++;
        if(coling > sideSize){
            coling = 0;
            if(temperature > minTemper) temperature -= 1;
        }

        if(curentSquer == 0) break;
    }
    // printSquer(out, sideSize);

    // printf("cost %d\n", squerCost(out, sideSize, goodSum));
}