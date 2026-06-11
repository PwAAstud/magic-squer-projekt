#include"setForce.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}

typedef struct{
    squerVal* p; // lista
    uint32_t n; // rozmiar
} SVslice;

typedef struct{
    squerVal* set;
    uint32_t searchSize; 
    uint32_t size;
} filteringSet;

static void printSquer(squerVal* squer, uint32_t sideSize){
    printf("\r\x1B[%dA", sideSize);
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%3d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
}

// true when return to start combination
static bool nextPerm(bool* list, uint32_t size){
    if(size == 0) return true;
    // 00 01 11 10
    uint32_t toSwap = 0;
    while(toSwap < size && !list[toSwap]) toSwap++;
    uint32_t pivot = toSwap;
    while(pivot < size && list[pivot]) pivot++;
    // printf("toSwap %d\npivot %d\n", toSwap, pivot);
    
    bool listReset = pivot == size;
    if(!listReset){
        list[pivot] = true;
        list[toSwap] = false;
    }
    uint32_t a = 0;
    uint32_t b = pivot-1;
    while (a < b && list[b] != list[a]){
        list[a] = true;
        list[b] = false;
        a++; b--;
    }
    return listReset;
}

// sprawdza czy sa zbiory liczb kture daja dobra sume. liczby ktura jest nieurzyta londuje na koniec
// po - pointer object - wstaznika na strukture
// sum - ile suma powina wynosic
// setSize - jaki jest rozmiar setu
// return - nowa dlugosc list
static void filterUnuseNumbers(filteringSet* po, uint32_t sum, uint32_t setSize){
    squerVal* list = po->set;
    uint32_t toCheck = po->searchSize, good = toCheck, size = po->size; // przedzialki listy
    bool inSet[size];

    while(toCheck > 0 && size >= setSize){
        bool setFindet = false;
        for(uint32_t i=0; i < setSize; i++) inSet[i] = true;
        for(uint32_t i=setSize; i < size; i++) inSet[i] = false;

        bool* permSet = inSet+1;
        uint32_t permSize = size-1;

        if(list[0] <= sum){
            do{
                uint32_t setSum = 0;
                for(uint32_t i=0; i < size; i++) if(inSet[i]) setSum += list[i];
                if(setSum == sum){
                    setFindet = true;
                    break;
                }
            }while(!nextPerm(permSet, permSize));
        }

        if(setFindet){
            for(uint32_t i = toCheck; i-- > 0;){
                if(!inSet[i]) continue;
                toCheck--;
                swap(list[i], list[toCheck]);
            }
        }else{
            // 0, toCheck, good, size
            toCheck--;
            swap(list[0], list[toCheck]);
            good--;
            swap(list[toCheck], list[good]);
            size--;
            swap(list[good], list[size]);
        }
    }
    for(uint32_t i = toCheck; i-- > 0;){
        // printf("%d\n", toCheck);
        toCheck--;
        swap(list[i], list[toCheck]);
        good--;
        swap(list[toCheck], list[good]);
    }

    po->searchSize = good;
}

static void doubleFaceFilter(filteringSet* po, uint32_t sum, uint32_t setSize, uint32_t sideSize){
    // if(setSize <= sideSize/4){
    if(setSize == 1){
        squerVal* set = po->set;
        uint32_t searchSize = po->searchSize;
        po->searchSize = 0;
        for(uint32_t i=0; i<searchSize; i++){
            if(set[i] != sum) continue;
            swap(set[0], set[i]);
            po->searchSize = 1;
            break;
        }
    // }else if(setSize > sideSize/2){
    // }else if(setSize > 3*sideSize/4){
    // }else if(setSize > sideSize/4){
    }else if(false){
        squerVal* set = po->set;
        uint32_t j = 0;
        for(uint32_t i=0; i<po->searchSize; i++){
            if(set[i] > sum) continue;
            swap(set[i], set[j]);
            j++;
        }
        po->searchSize = j;
    }else{
        filterUnuseNumbers(po,sum,setSize);
    }
}

static uint32_t specOrder(uint32_t i, uint32_t sideSize){
// 0 1 2 3
// 4 5 6 7
// 8 9 a b
// c d e f

// 0 1 2 3
// 4 7 8 9
// 5 a c d
// 6 b e f

// 0 1 2 3 y0 0
//   4 5 6 x0 1
//   7 8 9 y1 1
//     a b x1 2
//     c d y2 2
//       e x2 3
//       f y3 3
    uint32_t witchLine = 0;
    uint32_t lineLen = sideSize;
    for(;;){
        // y
        if(i < lineLen) return witchLine/2*sideSize + (i + (witchLine+1)/2);
        i -= lineLen;
        lineLen--;
        witchLine++;
        
        // x
        if(i < lineLen) return (i + (witchLine+1)/2)*sideSize + witchLine/2;
        i -= lineLen;
        witchLine++;
    }
    exit(1);
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
    uint32_t rightSetSize = sideSize;

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

    // for(uint32_t i=0;i<sideSize;i++) printf("\n");

    for(uint32_t i = 0; i < size;){
        printSquer(out, sideSize);

        // uint32_t cellId = i;
        uint32_t cellId = specOrder(i, sideSize);

        uint32_t y = cellId/sideSize;
        uint32_t x = cellId%sideSize;

        bool isLeft = x == y;
        bool isRight = y == sideSize - x - 1;

        if(out[cellId] != 0){
            squerVal remove = out[cellId];
            xSums[x] += remove;
            ySums[y] += remove;
            if(isLeft) leftSum += remove;
            if(isRight){
                rightSum += remove;
                rightSetSize++;
            }
            avalibe[remove] = true;
            out[cellId] = 0;

            if(cellOption[cellId].n == 0){
                i--;
                continue;
            }
        }

        if(cellOption[cellId].n == 0){
            squerVal* p = cellOption[cellId].p;
            uint32_t  n = cellOption[cellId].n;
            for (size_t i = 1; i <= size; i++){
                if(!avalibe[i]) continue;
                p[n++] = i;
            }
            filteringSet set = {.set = p, .size = n, .searchSize = n};
            doubleFaceFilter(&set, ySums[y], sideSize-x, sideSize);
            doubleFaceFilter(&set, xSums[x], sideSize-y, sideSize);
            if(isLeft)
                doubleFaceFilter(&set, leftSum, sideSize-y, sideSize);
            if(isRight)
                doubleFaceFilter(&set, rightSum, rightSetSize, sideSize);
            
            cellOption[cellId].n = set.searchSize;
            if(set.searchSize == 0){
                i--;
                continue;
            }
        }

        squerVal newVal = cellOption[cellId].p[--cellOption[cellId].n];
        xSums[x] -= newVal;
        ySums[y] -= newVal;
        if(isLeft) leftSum -= newVal;
        if(isRight){
            rightSum -= newVal;
            rightSetSize--;
        }
        avalibe[newVal] = false;
        out[cellId] = newVal;
        i++;
    }
    // printSquer(out, sideSize);

    free(cellOption[0].p);
}