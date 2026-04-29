#include"setForce.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define min(a,b) ((a <= b) ? a : b)
#define max(a,b) ((a >= b) ? a : b)
#define flat(x,y,w) (y*w+x)

typedef struct{
    squerVal* p; // lista
    uint32_t n; // rozmiar
} SVslice;

static void printSquer(squerVal* squer, uint32_t sideSize){
    printf("\r\x1B[%dA", sideSize);
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%3d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
}

// true when return to start combination
// do permutation backworts 0101 -> 0110
static bool nextPerm(bool* list, uint32_t size){
    // 00 01 11 10
    uint32_t toSwap = size-1;
    while(toSwap > 0 && !list[toSwap]) toSwap--;
    uint32_t pivot = toSwap;
    while(pivot > 0 && list[pivot]) pivot--;
    // printf("toSwap %d\npivot %d\n", toSwap, pivot);
    
    // bool listReset = list[toSwap] != 1 || list[pivot] != 0;
    bool listReset = !list[toSwap] || list[pivot];
    if(!listReset){
        list[pivot] = true;
        list[toSwap] = false;
    }
    uint32_t a = (listReset) ? 0 : pivot+1;
    uint32_t b = size-1;
    while (a < b && list[a]){
        list[a] = false;
        list[b] = true;
        // swap(list[a], list[b]);
        a++; b--;
    }
    return listReset;
}

// sprawdza czy sa zbiory liczb kture daja dobra sume. liczby ktura jest nieurzyta londuje na koniec
// list, size - lista wartosci oraz jej rozmiar, zalorzenie: list ma unikatowe wartosci
// sum - ile suma powina wynosic
// setSize - jaki jest rozmiar setu
// return - nowa dlugosc list
static uint32_t filterUnuseNumbers(squerVal* list, uint32_t listSize, uint32_t sum, uint32_t setSize){
    if(setSize == 0) return 0;
    setSize--; // jeden element jest zawsze juz dobrany do zbioru
    uint32_t accepted = 0; // rozmiar zaakceptowanych wartości, będą one na początku zmodyfikowanej listy
    bool inSet[listSize]; // lista pomocnicza mówiąc co jest w pozbiorze a co nie
    for(uint32_t i = listSize; i-- > accepted && i >= setSize;){
        if(list[i] > sum) continue;
        uint32_t newSum = sum - list[i];
    
        // # znajdz zbiur który ma dobra sume
        // inicjalizacja inSet
        for(uint32_t j=0; j < i; j++) inSet[j] = false;
        for(uint32_t j=i-setSize; j < i; j++) inSet[j] = true;

        // znalezienie zbioru
        bool setFindet = false;
        do{
            uint32_t setSum = 0;
            for(uint32_t j=0; j < i; j++) if(inSet[j]) setSum += list[j];
            if(setSum == newSum){
                setFindet = true;
                break;
            }
        }while(!nextPerm(inSet, i));
        
        if(!setFindet) continue;

        // ominięcie zaakceptowanych wartosc
        // while(inSet[accepted]) accepted++;
        
        for(uint32_t j = accepted; j < i; j++){
            if(!inSet[j]) continue;
            swap(list[j], list[accepted]);
            accepted++;
        }
        swap(list[i], list[accepted]);
        accepted++;
        i++;
    }
    
    return accepted;
}

void generateMagicSquer_setForce(squerVal* out, uint32_t sideSize){
    const uint32_t size = sideSize*sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    for(uint32_t i = 0; i < size; i++) out[i] = 0;
    uint32_t xSums[sideSize];
    uint32_t ySums[sideSize];
    for(uint32_t i = 0; i < sideSize; i++){
        xSums[i] = goodSum;
        ySums[i] = goodSum;
    }
    uint32_t leftSum = goodSum; // top left down right
    uint32_t rightSum = goodSum; // top right down left

    bool avalibe[size+1];
    avalibe[0] = false;
    for(uint32_t i = 0; i < size+1; i++) avalibe[i] = true;

    // inicjalizacja opcji
    SVslice cellOption[size];
    cellOption[0].p = malloc((size_t)sizeof(squerVal)*size*size);
    cellOption[0].n = 0;
    if(cellOption[0].p == NULL){
        fprintf(stderr, "%s:%d nie ma pamieci\n",__FILE__, __LINE__);
        exit(1);
    }
    for(uint32_t i = 1; i<size; i++){ 
        cellOption[i].p = cellOption[i-1].p + size;
        cellOption[i].n = 0;
    }

    for(uint32_t i = 0; i < size;){
        printSquer(out, sideSize);

        uint32_t cellId = i;

        uint32_t y = cellId/sideSize;
        uint32_t x = cellId%sideSize;

        bool isLeft = x == y;
        bool isRight = y == sideSize - x - 1;

        if(out[cellId] != 0){
            squerVal remove = out[cellId];
            xSums[x] += remove;
            ySums[y] += remove;
            if(isLeft) leftSum += remove;
            if(isRight) rightSum += remove;
            avalibe[remove] = true;
            out[cellId] = 0;
        }

        if(cellOption[cellId].n == 0){
            squerVal* p = cellOption[cellId].p;
            uint32_t  n = cellOption[cellId].n;
            for (size_t i = 1; i <= size; i++){
                if(!avalibe[i]) continue;
                p[n++] = i;
            }
            // TODO poprawic okreslanie setSize
            n = filterUnuseNumbers(p, n, ySums[y], sideSize-x);
            n = filterUnuseNumbers(p, n, xSums[x], sideSize-y);
            // if(isLeft)
            //     n = filterUnuseNumbers(p, n, leftSum, sideSize-y);
            // if(isRight)
            //     n = filterUnuseNumbers(p, n, rightSum, sideSize-y);
            
            cellOption[cellId].n = n;
            if(n == 0){
                i--;
                continue;
            }
        }

        squerVal newVal = cellOption[cellId].p[--cellOption[cellId].n];
        xSums[x] -= newVal;
        ySums[y] -= newVal;
        if(isLeft) leftSum -= newVal;
        if(isRight) rightSum -= newVal;
        avalibe[newVal] = false;
        out[cellId] = newVal;
        i++;
    }

    free(cellOption[0].p);
}