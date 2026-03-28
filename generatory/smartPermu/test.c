#include"smartPermu.c"

int main(){
    const uint32_t sideSize = 5;
    const uint32_t size = sideSize*sideSize;
    squerVal squer[size];
    generateMagicSquer_smartPermu(squer, sideSize);
}