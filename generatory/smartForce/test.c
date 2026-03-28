#include"smartForce.c"

int main(){
    const uint32_t sideSize = 4;
    const uint32_t size = sideSize*sideSize;
    squerVal squer[size];
    printf("good sum %d\n", sideSize*(sideSize*sideSize + 1)/2);
    generateMagicSquer_smartForce(squer, sideSize);
}