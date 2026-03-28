#include"force.h"
#include<stdlib.h>
#include<stdbool.h>

#include<stdio.h>
static void printSquer(squerVal* squer, uint32_t sideSize){
    printf("\r\x1B[%dA", sideSize+1);
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%3d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    printf("\n");
}

// typedef struct{
//     squerVal* squer;
//     bool* ocupade;
//     uint32_t* xSums;
//     uint32_t* ySums;
//     uint32_t crosSum;

//     uint32_t sideSize;
// } squerData;

void generateMagicSquer_force(squerVal* out, uint32_t sideSize){
    const uint32_t size = sideSize*sideSize;
    for(uint32_t i = 0; i < size; i++) out[i] = 0;

    // +2 poniewasz pierwszy jest nie urzywany a ostanie jest aby zatrzymac petle
    bool* ocupade = malloc( (size+2) *sizeof(bool));
    for(uint32_t i = 0; i < size+2; i++) ocupade[i] = false;

    const uint32_t goodSum = sideSize*(sideSize*sideSize + 1)/2;
    // printf("good sum: %d\n\n", goodSum);
    uint32_t* xSums = malloc(sideSize*sizeof(uint32_t));
    uint32_t* ySums = malloc(sideSize*sizeof(uint32_t));
    uint32_t  leftSum = goodSum; // top left down right
    uint32_t  rightSum = goodSum; // top right down left
    for(uint32_t i = 0; i < sideSize; i++){
        xSums[i] = goodSum;
        ySums[i] = goodSum;

        // printf("\n");
    }
    for(uint32_t i = 0; i < size;){
        // printf("%3d/%3d\r", i, size);
        // for(uint32_t a = 0; a < sideSize; a++) printf("%2d ", xSums[a]);
        // printf("\n");
        // for(uint32_t a = 0; a < sideSize; a++) printf("%2d ", ySums[a]);
        // printf("\n");
        // printf("%d %d\n", leftSum, rightSum);
        
        // printSquer(out, sideSize);
        
        uint32_t x = i%sideSize, y = i/sideSize; 
        // printf("x:%d y:%d\n",x, y);
        squerVal newNum;
        if(out[i] == 0 && (x == sideSize-1 || y == sideSize-1) ){
            if(y == sideSize-1) {
                if(x == sideSize-1) newNum = leftSum;
                else if(x == 0) newNum = rightSum;
                else newNum = xSums[x];
            }
            else newNum = ySums[y];

            if(newNum == 0) newNum = size + 1;
        }else{
            newNum = out[i] + 1;
            while(ocupade[newNum]) newNum++;
        }
        // printf("new num %d\n\n", newNum);

        bool isLeft = x == y;
        bool isRight = y == sideSize - x - 1;

        ocupade[out[i]] = false;
        xSums[x] += out[i];
        ySums[y] += out[i];
        if(isLeft) leftSum += out[i];
        if(isRight) rightSum += out[i];
        out[i] = 0;

        if(
            newNum > size
        ||  ocupade[newNum]
        ||  newNum > xSums[x]
        ||  newNum > ySums[y]
        ||  isLeft && newNum > leftSum
        ||  isRight && newNum > rightSum
        ){
            i--;
        }else{
            ocupade[newNum] = true;
            out[i]    = newNum;
            xSums[x] -= newNum;
            ySums[y] -= newNum;
            if(isLeft) leftSum -= out[i];
            if(isRight) rightSum -= out[i];
            i++;
        }
    }


    // for(uint32_t a = 0; a < sideSize; a++) printf("%2d ", xSums[a]);
    // printf("\n");
    // for(uint32_t a = 0; a < sideSize; a++) printf("%2d ", ySums[a]);
    // printf("\n");
    // printSquer(out, sideSize);

    free(ocupade); free(xSums); free(ySums);
}