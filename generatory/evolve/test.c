#include"evolve.c"

static void printSquer(squerVal* squer, uint32_t sideSize){
    for(uint32_t y = 0; y < sideSize; y++){
        for(uint32_t x = 0; x < sideSize; x++) printf("%4d", squer[x]);
        squer += sideSize;
        printf("\n");
    }
    // printf("\n");
}

void test1(){
    srand(time(NULL));
    const uint32_t sideSize = 3;
    const uint32_t size = sideSize*sideSize;
    
    squerVal squer1[size];
    squerVal squer2[size];
    randSquer(squer1, size);
    randSquer(squer2, size);

    printSquer(squer1, sideSize);
    printf("\n");
    printSquer(squer2, sideSize);
    printf("\n");

    squerVal squer3[size];
    mixSquer(squer3, squer1, squer2, size);
    printSquer(squer3, sideSize);
}

// static test2()

int main(){
    // test1();
    srand(time(NULL));
    const uint32_t sideSize = 8;
    const uint32_t size = sideSize*sideSize;
    
    squerVal squer1[size];
    generateMagicSquer_evolve(squer1, sideSize);

    printSquer(squer1, sideSize);
}