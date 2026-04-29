#include<stdio.h>
#include"setForce.c"

void testPerm(){
    bool list[] = {true, true, false, false, false, false};

    int size = sizeof(list)/sizeof(bool);
    do{
        for(int i=0;i<size;i++){
            printf("%d ", list[i]);
        }
        printf("\n");
    }while(!nextPerm(list, size));
}

void testFilter(){
    squerVal list[] = {5,4,3,2};
    long size = sizeof(list)/sizeof(*list);
    filteringSet set = {.set = list, .size = size, .searchSize = size};

    filterUnuseNumbers(&set, 5, 3);
    filterUnuseNumbers(&set, 6, 2);

    // printf("set.searchSize %d\n", set.searchSize);
    for(int i=0;i<set.searchSize;i++) printf("%d ", list[i]); printf("\n");
    for(int i=0;i<set.size;i++) printf("%d ", list[i]); printf("\n");
}

void test3(){
    const uint32_t sideSize = 8;
    const uint32_t size = sideSize*sideSize;
    squerVal squer[size];
    generateMagicSquer_setForce(squer, sideSize);
}

int main(){
    // testPerm();
    // testFilter();
    test3();

    return 0;
}