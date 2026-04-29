#include"probForce.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define min(a,b) ((a <= b) ? a : b)
#define max(a,b) ((a >= b) ? a : b)
#define flat(x,y,w) (y*w+x)

typedef struct{
    // ctxlen; prob
    uint32_t* prob;
    uint32_t ctxMaxSize;
    uint32_t probLen;
} ctx;

// true when return to start combination
static bool nextPerm(bool* list, uint32_t size){
    // 00 01 11 10
    uint32_t toSwap = 0;
    while(toSwap < size && !(list[toSwap])) toSwap++;
    uint32_t pivot = toSwap;
    while(pivot < size && list[pivot]) pivot++;

    if(pivot < size) swap(list[pivot], list[toSwap]);
    uint32_t a = 0;
    uint32_t b = pivot-1;
    while (a < b && list[b]){
        swap(list[a], list[b]);
        a++; b--;
    }
    return !(pivot < size);
}

static bool nextGoodPerm(bool* list, uint32_t size, int goodSum){
    uint32_t sum;
    do{
        if(nextPerm(list, size)) return true;
        sum = 0;
        for(uint32_t i = 0; i < size; i++){
            if(!list[i]) continue;
            sum += 1 + i;
        }
    }while(sum != goodSum);
    return false;
}

static uint32_t combin(uint32_t n, uint32_t k){
    // https://en.wikipedia.org/wiki/Binomial_coefficient
    uint32_t retVal = 1;
    for(uint32_t i = 1; i <= k; i++){
        retVal *= n--;
        retVal /= i;
    }
    return retVal;
}

static void ctx_print(ctx o){
    for(uint32_t ctxSize = 0; ctxSize <= o.ctxMaxSize; ctxSize++){
        bool chosen[o.probLen];
        for(uint32_t i = 0; i < o.probLen; i++) chosen[i] = false;
        for(uint32_t i = 0; i < ctxSize; i++) chosen[i] = true;

        do {
            printf("{");
            for(uint32_t j = 0; j < o.probLen; j++) if(chosen[j]) printf(" %2d", j+1);
            printf("}\n");
            for(uint32_t j = 0; j < o.probLen; j++) printf("%2d ", *(o.prob++));
            printf("\n");
        } while(!nextPerm(chosen, o.probLen));
    }
}

static ctx ctx_init(uint32_t ctxMaxSize, uint32_t probLen){
    ctx retVal = {.ctxMaxSize = ctxMaxSize, .probLen = probLen};
    
    // meme area for all prob lists
    size_t sumLen = 0;
    for(uint32_t i = 0; i <= ctxMaxSize; i++) sumLen += combin(probLen, i);
    
    // prob list są ustawione w 

    retVal.prob = calloc(sumLen*probLen,sizeof(uint32_t));
    // printf("sumLen: %ld; aloc size: %ld\n", sumLen, sumLen*probLen);
    if(retVal.prob == NULL){
        fprintf(stderr, "%s:%d nie ma pamieci\n",__FILE__, __LINE__);
        exit(1);
    }

    return retVal;
}

// set ma byc posortowane rosnaco
static uint32_t* ctx_getProb(ctx o, uint32_t* set, uint32_t setSize){
    uint32_t retI = 0;
    for(uint32_t i = 0; i < setSize; i++) retI += combin(o.probLen, i);
    uint32_t k = setSize;
    for(uint32_t i = setSize; i-- > 0;){
        if(set[i]-1 < k) break;
        retI += combin(set[i]-1, k);
        k--;
    }
    return o.prob + o.probLen*retI;
}

// set ma byc posortowane rosnaco
static void ctx_addSet(ctx o, uint32_t* set, uint32_t setSize){
    bool withCtx[setSize];
    uint32_t ctxSet[setSize];

    for(uint32_t ctxSize = 0; ctxSize < setSize && ctxSize <= o.ctxMaxSize; ctxSize++){
        for(uint32_t i = 0; i < setSize; i++) withCtx[i] = false;
        for(uint32_t i = 0; i < ctxSize; i++) withCtx[i] = true;

        for(;;){
            // robienie contextu dla getProb
            uint32_t ctxSetI = 0;
            for (uint32_t i = 0; i < setSize; i++) if(withCtx[i]) ctxSet[ctxSetI++] = set[i];
            
            uint32_t* probList = ctx_getProb(o, ctxSet, ctxSetI);

            for (uint32_t i = 0; i < setSize; i++) if(!withCtx[i]) probList[set[i]-1]++;

            if(nextPerm(withCtx, setSize)) break;    
        }
    }
}

static void ctx_free(ctx toFree){
    free(toFree.prob);
}

// generuje strukturę która ma przechowywać prawdopobieństwa znaków dla konkretnego kontekstu
static ctx create_probStruc(uint32_t sideSize, uint32_t ctxMaxSize){
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    bool chosen[size];
    for(uint32_t i = 0; i < size; i++) chosen[i] = false;
    for(uint32_t i = 0; i < sideSize; i++) chosen[i] = true;

    ctx probStruc = ctx_init(ctxMaxSize, size);
    for(;;){
        if(nextGoodPerm(chosen, size, goodSum)) break;

        uint32_t setI = 0;
        uint32_t set[sideSize];
        // wyswietl wersje
        for(int i=0; i < size; i++){
            if(chosen[i] == 0) continue;
            // printf("%2d ", i+1);
            set[setI++] = i+1;
        }
        // printf("\n");
        ctx_addSet(probStruc, set, sideSize);
    }
    return probStruc;
}

static void sortSet(uint32_t* list, uint32_t num){
    for(uint32_t i = 1; i < num; i++){
        for(uint32_t j = i; j > 0; j--){
            if(list[j-1] <= list[j]) break;
            swap(list[j-1], list[j]);
        }
    }
    // printf("{");
    // for(uint32_t i = 0; i < num; i++) printf("%2d ", list[i]);
    // printf("}\n");
}

static uint32_t findBestProb(uint32_t* probList, uint32_t size){
    uint32_t retVal = 0;
    for(uint32_t i = 1; i < size; i++) if(probList[i] > probList[retVal]) retVal = i;
    return retVal;
}

static void printSquer(squerVal* squer, uint32_t sideSize){
    printf("\x1B[%dA", sideSize);
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%3d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    // printf("\n");
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

void generateMagicSquer_probForce(squerVal* out, uint32_t sideSize){
    const int32_t ctxMaxSize = min(4, sideSize-2);
    const uint32_t size = sideSize * sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;
    ctx probStruc = create_probStruc(sideSize, ctxMaxSize);

    // ctx_print(probStruc);
    for(uint32_t i = 0; i < size; i++) out[i] = 0;
    uint32_t* cellProb =  malloc(size*size*sizeof(uint32_t));
    if(cellProb == NULL){
        fprintf(stderr, "%s:%d; nie ma pamieci",__FILE__, __LINE__);
        exit(1);
    }
    bool* avalibe = malloc((size+1)*sizeof(bool));
    for(uint32_t i = 0; i < size+1; i++) avalibe[i] = true;
    avalibe[0] = false;
    // for(uint32_t i=0;i<sideSize;i++) printf("\n");
    for(uint32_t i = 0; i <= size;){
        // sprawdzanie przekatnych
        if(i == size){
            uint32_t sum = 0;
            for(uint32_t i=0;i<sideSize;i++) sum += out[i*sideSize+i];
            if(sum != goodSum){
                i--;
                continue;
            }
            sum = 0;
            for(uint32_t i=0;i<sideSize;i++) sum += out[i*sideSize+(sideSize-1-i)];
            if(sum != goodSum){
                i--;
                continue;
            }

            i++;
            continue;
        }
        // printSquer(out, sideSize);
        
        uint32_t ii = specOrder(i, sideSize);
        // uint32_t ii = i;

        int32_t x = ii%sideSize, y = ii/sideSize; 
        squerVal newNum = 0;

        // czy nowe pole czy backtrack
        if(out[ii] == 0){
            if(y == sideSize-1 || x == sideSize-1){
                if(y == sideSize-1){
                    for(uint32_t yi = 0; yi < sideSize-1; yi++) newNum += out[flat(x,yi,sideSize)];
                }else{
                    for(uint32_t xi = 0; xi < sideSize-1; xi++) newNum += out[flat(xi,y,sideSize)];
                }
                if(newNum >= goodSum) newNum = 0;
                else{
                    newNum = goodSum - newNum;
                    if(newNum > size) newNum = 0;
                }
            }else{
                uint32_t* curentCellProb = cellProb + size*i;
                for (size_t i = 0; i < size; i++) curentCellProb[i] = avalibe[i+1];
                
                uint32_t setCtx[ctxMaxSize];
                // x
                uint32_t setSize = 0;
                for(uint32_t xi = max(x, ctxMaxSize)-ctxMaxSize; xi < x; xi++){
                    setCtx[setSize++] = out[flat(xi,y,sideSize)];
                }
                // printf("xi %d\n", max(x-ctxMaxSize,0));
                sortSet(setCtx, setSize);
                uint32_t* prob = ctx_getProb(probStruc, setCtx, setSize); 
                for(uint32_t a=0;a<size;a++) curentCellProb[a] *= prob[a];

                // y
                setSize = 0;
                for(uint32_t yi = min(y, ctxMaxSize); yi-- > 0; ){
                    setCtx[setSize++] = out[flat(x,yi,sideSize)];
                }
                sortSet(setCtx, setSize);
                prob = ctx_getProb(probStruc, setCtx, setSize);
                for(uint32_t a=0;a<size;a++) curentCellProb[a] *= prob[a];

                newNum = findBestProb(curentCellProb, size);
                if(curentCellProb[newNum] == 0){
                    newNum = 0;
                }else{
                    curentCellProb[newNum] = 0;
                    newNum++;
                }
            }
        }else{
            if(y == sideSize-1 || x == sideSize-1){
                newNum = 0;
            }else{
                // printf("d\n");
                uint32_t* curentCellProb = cellProb + size*i;
                // for(uint32_t a=0;a<size;a++) printf("%d ", curentCellProb[a]); printf("\n");
                newNum = findBestProb(curentCellProb, size);
                if(curentCellProb[newNum] == 0){
                    newNum = 0;
                }else{
                    curentCellProb[newNum] = 0;
                    newNum++;
                }
            }
        }
        // printf("new num %d\n", newNum);

        if(out[ii] != 0){
            avalibe[out[ii]] = true;
            out[ii] = 0;
        }
        if(newNum == 0 || avalibe[newNum] == false){
            i--;
            continue;
        }
        avalibe[newNum] = false;
        out[ii] = newNum;
        i++;
    }

    // printSquer(out, sideSize);

    free(cellProb);
    free(avalibe);
    ctx_free(probStruc);
}