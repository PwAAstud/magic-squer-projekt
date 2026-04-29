#include"rowFilter.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#define swap(a,b) {typeof(a) t = a; a = b; b = t;}
#define numMemAlloc(type, num) malloc(sizeof(type)*num);

// line go from start to squer end in direction of row or colm
typedef struct{
    uint32_t* index;
    uint32_t* val;
    uint32_t size;
    uint32_t* sum;
} lineData;

typedef struct{
    bool* avalibe; //  i+1 to faktyczna wartosc; 1 avalibe if avalibe[0]==true
    uint32_t avalSize;
    uint32_t goodSum;

    uint32_t sideSize;
    uint32_t* xSum;
    uint32_t* ySum;
    uint32_t  leftSum;
    uint32_t  rightSum;
} squerState;

// true when return to start combination
static bool nextPerm(uint32_t* list, uint32_t size){
    if(size <= 1) return true;

    uint32_t pivot = 1;
    while(pivot < size && list[pivot-1] >= list[pivot]) pivot++;
    bool nextPermExist = pivot < size;

    if(nextPermExist){
        uint32_t toSwap = 0;
        while(list[toSwap] >= list[pivot]) toSwap++;
        swap(list[toSwap], list[pivot]);
    }
    
    uint32_t a = 0;
    uint32_t b = pivot-1;
    while (a < b){
        swap(list[a], list[b]);
        a++; b--;
    }
    return !nextPermExist;
}

static lineData* generateLineList(squerState state){
    const uint32_t size = state.sideSize*state.sideSize;
    
    uint32_t* indexList = numMemAlloc(uint32_t, size);
    uint32_t* valList = numMemAlloc(uint32_t, size);

    lineData* retVal = numMemAlloc(lineData, 2*state.sideSize-1);

    // linie
    // 0 0 0 0
    // 1 2 2 2
    // 1 3 4 4
    // 1 3 5 6
    // const y: 0 2 4 6; const x: 1 3 5

    // const y
    for(uint32_t y = 0; y < state.sideSize; y++){
        lineData newLine = {
            .index = indexList, 
            .val = valList, 
            .size = state.sideSize-y,
            .sum = state.ySum + y
        };
        indexList += newLine.size;
        valList += newLine.size;

        for(uint32_t x = 0; x < newLine.size; x++){
            newLine.index[x] = y*state.sideSize + (y+x);
        }

        retVal[2*y] = newLine;
    }

    // const x
    for(uint32_t x = 0; x < state.sideSize-1; x++){
        lineData newLine = {
            .index = indexList, 
            .val = valList, 
            .size = state.sideSize-x-1,
            .sum = state.xSum + x
        };
        indexList += newLine.size;
        valList += newLine.size;

        for(uint32_t y = 0; y < newLine.size; y++){
            newLine.index[y] = (y+x+1)*state.sideSize + x;
        }

        retVal[2*x+1] = newLine;
    }

    return retVal;
}

static void freeLineList(lineData* list){
    free(list[0].index);
    free(list[0].val);
    free(list);
}

// true then end
static bool nextLine(lineData line, const squerState state){
    // suma jest poprawna (zalorznie) wiec nextPerm jest poprawne 
    // jak nie szukamy nowej kombinacji
    if(!nextPerm(line.val, line.size)) return false;

    uint32_t chosen[state.avalSize];
    // 1 to false; 0 to true; jest tak aby nextPerm dzialalo
    for(uint32_t i=0; i<state.avalSize; i++) chosen[i] = 1;
    for(uint32_t i=0; i<line.size; i++) chosen[line.val[i]-1] = 0;
    
    uint32_t avalVal[state.avalSize];
    uint32_t avalSize = 0;
    for(uint32_t i=0; i<state.avalSize; i++){
        if(state.avalibe[i] == false) continue;
        chosen[avalSize] = chosen[i];
        avalVal[avalSize] = i+1;
        avalSize++;
    }

    while (!nextPerm(chosen, avalSize)){
        uint32_t sum = 0;
        for(uint32_t i=0; i<avalSize; i++){
            if(chosen[i] == 1) continue;
            sum += avalVal[i];
        }
        if(sum == *line.sum){
            uint32_t j = 0;
            for(uint32_t i=0; i<avalSize; i++){
                if(chosen[i] == 1) continue;
                line.val[j++] = avalVal[i];
            }
            return false;
        } 
    }
    return true;
}

// true then end
static bool initLine(lineData line, const squerState state){
    uint32_t avalVal[state.avalSize];
    uint32_t avalSize = 0;
    for(uint32_t i=0; i<state.avalSize; i++){
        if(state.avalibe[i] == false) continue;
        avalVal[avalSize] = i+1;
        avalSize++;
    }

    uint32_t chosen[avalSize];
    // 1 to false; 0 to true; jest tak aby nextPerm dzialalo
    for(uint32_t i=0; i<avalSize; i++) chosen[i] = 1;
    for(uint32_t i=0; i<line.size; i++) chosen[i] = 0;
    
    do{
        // for(uint32_t i=0; i<avalSize; i++) printf("%d ",chosen[i]);
        // printf("line.size:%d\n",line.size);
        uint32_t sum = 0;
        for(uint32_t i=0; i<avalSize; i++){
            if(chosen[i] == 1) continue;
            sum += avalVal[i];
        }
        if(sum == *line.sum){
            uint32_t j = 0;
            for(uint32_t i=0; i<avalSize; i++){
                if(chosen[i] == 1) continue;
                // printf("%d ", avalVal[i]);
                line.val[j++] = avalVal[i];
                // line.val[j++] = 1;
            }
            // printf("j: %d\n", j);
            return false;
        }
    }while (!nextPerm(chosen, avalSize));
    return true;
}

static bool canApplyLine(const lineData line, const squerState state){
    for (uint32_t i = 0; i < line.size; i++){
        uint32_t y = line.index[i] / state.sideSize;
        uint32_t x = line.index[i] % state.sideSize;

        bool isLeft = x == y;
        bool isRight = y == state.sideSize-1-x;

        uint32_t val = line.val[i];
        if(state.xSum[x] < val) return false;
        if(state.ySum[y] < val) return false;
        if(isLeft && state.leftSum < val) return false;
        if(isRight && state.rightSum < val) return false;
    }
    return true;
}

static void applyLine(lineData line, squerState* pState){
    squerState state = *pState;
    for (uint32_t i = 0; i < line.size; i++){
        uint32_t y = line.index[i] / state.sideSize;
        uint32_t x = line.index[i] % state.sideSize;

        bool isLeft = x == y;
        bool isRight = y == state.sideSize-1-x;

        uint32_t val = line.val[i];
        state.xSum[x] -= val;
        state.ySum[y] -= val;
        if(isLeft) state.leftSum -= val;
        if(isRight) state.rightSum -= val;
        state.avalibe[val-1] = false;
    }
    *pState = state; 
}

static void removeLine(lineData line, squerState* pState){
    squerState state = *pState;
    for (uint32_t i = 0; i < line.size; i++){
        uint32_t y = line.index[i] / state.sideSize;
        uint32_t x = line.index[i] % state.sideSize;

        bool isLeft = x == y;
        bool isRight = y == state.sideSize-1-x;

        uint32_t val = line.val[i];
        state.xSum[x] += val;
        state.ySum[y] += val;
        if(isLeft) state.leftSum += val;
        if(isRight) state.rightSum += val;
        state.avalibe[val-1] = true;
    }
    *pState = state; 
}

void generateMagicSquer_rowFilter(squerVal* out, uint32_t sideSize){
    const uint32_t size = sideSize*sideSize;
    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;

    bool avalibe[size];
    for(uint32_t i=0;i<size;i++) avalibe[i] = true;
    uint32_t xSum[sideSize];
    uint32_t ySum[sideSize];
    for(uint32_t i=0;i<sideSize;i++){
        xSum[i] = goodSum;
        ySum[i] = goodSum;
    }

    squerState state = {
        .avalibe = avalibe, .avalSize = size,
        .goodSum = goodSum,
        .sideSize = sideSize,

        .leftSum = goodSum,
        .rightSum = goodSum,
        .xSum = xSum, .ySum = ySum
    };

    const uint32_t lineListSize = 2*sideSize-1;
    lineData* lineList = generateLineList(state);

    bool lineApplay[lineListSize];
    for(uint32_t i=0;i<lineListSize;i++) lineApplay[i] = false;

    for(uint32_t i=0; i<lineListSize;){
        // for(uint32_t j=0;j<size;j++) printf("%d ", state.avalibe[j]);
        // printf("\n");
        printf("%2d\r",lineListSize-i);
        bool nextHapen;
        if(lineApplay[i] == true){
            lineApplay[i] = false;
            removeLine(lineList[i], &state);
            nextHapen = nextLine(lineList[i], state);
        }else{
            nextHapen = initLine(lineList[i], state);
        }
        if(nextHapen){
            i--;
            continue;
        }

        nextHapen = false;
        while (!canApplyLine(lineList[i], state)){
            if(nextLine(lineList[i], state)){
                nextHapen = true;
                break;
            }
        }
        if(nextHapen){
            i--;
            continue;
        }

        lineApplay[i] = true;
        applyLine(lineList[i], &state);
        i++;

        if(i == lineListSize && (state.leftSum != 0 || state.rightSum != 0)) i--;
    }

    for(uint32_t i=0; i<lineListSize; i++){
        lineData line = lineList[i];
        for(uint32_t j=0; j < line.size; j++)
            out[line.index[j]] = line.val[j];
    }

    free(lineList);
}