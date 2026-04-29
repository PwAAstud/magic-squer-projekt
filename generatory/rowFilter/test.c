#include"rowFilter.c"
#include<stdio.h>

void testPerm(){
    uint32_t list[] = {1,2,3,4,5};
    uint32_t size = sizeof(list)/sizeof(*list);
    
    do{
        for(uint32_t i=0;i<size; i++) printf("%d ", list[i]);
        printf("\n");
    } while(!nextPerm(list, size));
}

void testGenList(){
    // const uint32_t d = 6;
    // lineData* list = generateLineList(d);
    // for(uint32_t i = 0; i < 2*d-1; i++){
    //     for(uint32_t j=0; j<list[i].size; j++){
    //         printf("%d ", list[i].index[j]);
    //     }
    //     printf("\n");
    // }
    // free(list);
}

void testNextLine(){
    uint32_t val[3];
    lineData line = {.val = val, .size = 2};

    bool aval[] = {true, true, true, true, true, true, true, true, true, true, true, true};
    squerState state = {.avalibe = aval, .avalSize = 5, .goodSum = 6};

    if(initLine(line, state)) return;
    do{
        for(uint32_t i=0; i<line.size; i++) printf("%d ", val[i]);
        printf("\n");
    }while(!nextLine(line, state));
}

void testGenerator(){
    const int sideSize = 6;
    squerVal squer[sideSize*sideSize];
    generateMagicSquer_rowFilter(squer, sideSize);

    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) 
            printf("%4d", squer[y*sideSize + x]);
        printf("\n");
    }

}

int main(){
    testGenerator();
    // testPerm();
    // testGenList();
    // testNextLine();

    return 0;
}