#include"evolve.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<string.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define dif(a, b) ((a >= b) ? a-b : b-a)
#define error(s) {printf("%s:%d %s\n", __FILE__, __LINE__, s); exit(1);}

#define numMemAlloc(type, name, num) type* name = malloc(sizeof(type)*num);

typedef struct{
    squerVal* l;
    uint32_t cost;
} squer;

static void printSquerA(squerVal* squer, uint32_t sideSize){
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%4d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    // printf("\n");
}

static void mixSquer(squerVal* out, squerVal* squerA, squerVal* squerB, uint32_t size){
    bool taken[size+1];
    memset(taken, false, (size+1)*sizeof(bool));
    uint32_t posB[size+1];

    uint32_t cp1 = rand()%size;
    uint32_t cp2 = rand()%size;
    if(cp2 < cp1) swap(cp1, cp2);
    // printf("%d %d\n", cp1, cp2);

    for (uint32_t i = 0; i < size; i++) {
        posB[squerB[i]] = i;
    }
    memset(out, 0, size * sizeof(squerVal));

    for(uint32_t i=cp1; i < cp2; i++){
        squerVal valA = squerA[i];
        taken[valA] = true;
        out[i] = valA;
    }

    for(uint32_t i=cp1; i < cp2; i++){
        squerVal valB = squerB[i];
        if(taken[valB]) continue;
        uint32_t placeIn = i;
        do{
            placeIn = posB[squerA[placeIn]];
        }while (cp1 <= placeIn && placeIn < cp2);
        out[placeIn] = valB;
    }

    for(uint32_t i=0; i<size; i++){
        if(out[i] != 0) continue;
        out[i] = squerB[i];
    }
    

    // if(true || rand()%2 < 1){
    uint32_t ia = rand()%size;
    uint32_t ib = rand()%size;
    swap(out[ia], out[ib]);
    // }
}

static void copySquer(squerVal* out, squerVal* squer, uint32_t size){
    for(uint32_t i = 0; i < size; i++) out[i] = squer[i];
    
    uint32_t ia = rand()%size;
    uint32_t ib = rand()%size;
    swap(out[ia], out[ib]);
}

static void randSquer(squerVal* out, uint32_t size){
    for(uint32_t i=0; i<size; i++) out[i] = i+1;
    for(uint32_t i = size; i > 0;){
        uint32_t r = rand()%i;
        i--;
        swap(out[i], out[r]);
    }
}

static uint32_t squerCost(squerVal* list, uint32_t sideSize, uint32_t goodSum){
    uint32_t retVal = 0;
    uint32_t sum = 0;
    squerVal* reedFrom;
    // row
    for(uint32_t y = 0; y < sideSize; y++){
        sum = 0;
        reedFrom = list + y*sideSize;
        for(uint32_t x = 0; x < sideSize; x++) sum += *(reedFrom++);
        retVal += dif(sum, goodSum);
    }

    // colm
    for(uint32_t x = 0; x < sideSize; x++){
        sum = 0;
        reedFrom = list + x;
        for(uint32_t x = 0; x < sideSize; x++){
            sum += *reedFrom;
            reedFrom += sideSize;
        } 
        retVal += dif(sum, goodSum);
    }

    // left
    sum = 0;
    reedFrom = list;
    for(uint32_t i = 0; i < sideSize; i++){
        sum += *reedFrom;
        reedFrom += sideSize + 1;
    }
    retVal += dif(sum, goodSum);

    // right
    sum = 0;
    reedFrom = list + sideSize - 1;
    for(uint32_t i = 0; i < sideSize; i++){
        sum += *reedFrom;
        reedFrom += sideSize - 1;
    }
    retVal += dif(sum, goodSum);

    return retVal;
}

// <!!! załorzenie kada operacja albo dzieli polulacja przez dwa albo mnorzy przez daw
// tak załorzenie może problematyncze później

static void saveBestPops(squer* pops, uint32_t popsSize, uint32_t target){
    for(uint32_t j=0; j < target; j++){
        uint32_t thisBestPop = UINT32_MAX;
        uint32_t bestI;
        for(uint32_t i=j; i < popsSize; i++){
            if(pops[i].cost < thisBestPop){
                thisBestPop = pops[i].cost;
                bestI = i;
            }
        }
        swap(pops[bestI], pops[j]);
    }
}

static void removeBestPops(squer* pops, uint32_t* popsSize_p){
    for(uint32_t step = *popsSize_p/2; step > 0; step/=2){
        for(uint32_t a = step; a < *popsSize_p; a++){
            for (uint32_t b = a; b >= step; b-=step){
                uint32_t ib = b-step;
                if(pops[ib].cost >= pops[b].cost) break;
                swap(pops[ib], pops[b]);
            }
        }
    }

    *popsSize_p /= 2;
}

static void selectPopsTormament(squer* pops, uint32_t* popsSize_p){
    uint32_t popsSize = *popsSize_p;

    for(uint32_t i = popsSize; i > 0;){
        uint32_t r = rand()%i;
        i--;
        swap(pops[i], pops[r]);
    }

    uint32_t newPopsSize = 0;
    for(uint32_t i = 0; i < popsSize; i+=2){
        uint32_t newPop = 0;
        if(pops[i].cost > pops[i+1].cost) newPop++;
        // if(pops[i].cost!=0 && rand()%128 < 1) newPop != newPop;

        swap(pops[newPop+i], pops[newPopsSize]);
        newPopsSize++;
    }

    *popsSize_p = newPopsSize;
}

static void generatePops(squer* pops, uint32_t* popsSize_p, uint32_t sideSize){
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;
    
    uint32_t popsSize = *popsSize_p;
    for(uint32_t i = popsSize; i > 0;){
        uint32_t r = rand()%i;
        i--;
        swap(pops[i], pops[r]);
    }

    squer newSquer = pops[popsSize];

    mixSquer(newSquer.l, pops[popsSize-1].l, pops[0].l, size);
    newSquer.cost = squerCost(newSquer.l, sideSize, goodSum);
    pops[popsSize++] = newSquer;
    for (uint32_t i = popsSize-1; i > 1; i--){
        squer newSquer = pops[popsSize];
        mixSquer(newSquer.l, pops[i-1].l, pops[i-2].l, size);
        newSquer.cost = squerCost(newSquer.l, sideSize, goodSum);
        pops[popsSize++] = newSquer;
    }

    // fragment kodu dla reprodukcji asueksulanej
    // for (uint32_t i = popsSize; i-- > 0; ){
    //     squer newSquer = pops[popsSize];
    //     copySquer(newSquer.l, pops[i].l, size);
    //     newSquer.cost = squerCost(newSquer.l, sideSize, goodSum);
    //     pops[popsSize++] = newSquer;
    // }

    *popsSize_p = popsSize;
}

// !!!>

void generateMagicSquer_evolve(squerVal* out, uint32_t sideSize){
    srand(time(NULL));
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    // uint32_t popSize = sideSize*750;
    uint32_t popSize = 5000;
    
    numMemAlloc(squerVal, popsArea, popSize*size); 
    if(!popsArea) error("pamieci brak");
    numMemAlloc(squer, pops, popSize);
    if(!pops) error("pamieci brak");
    for(uint32_t i = 0; i < popSize;i++){
        pops[i].l = popsArea + i*size;
        randSquer(pops[i].l, size);
        pops[i].cost = squerCost(pops[i].l, sideSize, goodSum);
    }

    for(;true;){
        for(uint32_t i = 0; i < popSize;i++){
            randSquer(pops[i].l, size);
            pops[i].cost = squerCost(pops[i].l, sideSize, goodSum);
        }
        
        uint32_t bestPop = UINT32_MAX;
        for(uint32_t i=0; i < 1000; i++){
            selectPopsTormament(pops, &popSize);
            uint32_t oldSize = popSize;
            // for(uint32_t i=0; i < popSize; i++) printf("%d ", pops[i].cost); printf("\n");
            generatePops(pops, &popSize, sideSize);
            
            uint32_t thisBestPop = UINT32_MAX;
            for(uint32_t j=0; j < popSize; j++){
                if(pops[j].cost < thisBestPop) thisBestPop = pops[j].cost;
            }
            if(bestPop == 0) break;
            if(thisBestPop < bestPop){
                // printf("%d ", thisBestPop);
                // fflush(stdout);
                i = 0;
            }
            bestPop = thisBestPop;
            // printf("%d\r", i);
        }
        if(bestPop == 0) break;
    }
    
    // for(uint32_t i=0; i < popSize; i++){
    //     printf("%d ", pops[i].cost);
    // }
    // printf("\n");
    
    uint32_t retI = 0;
    for(uint32_t i=1; i < popSize; i++){
        if(pops[i].cost < pops[retI].cost) retI = i;
    }
    // printf("koszt rozwiazania %d\n", pops[retI].cost);

    for(uint32_t i=0; i < size; i++) out[i] = pops[retI].l[i];

    free(popsArea);
    free(pops);
}