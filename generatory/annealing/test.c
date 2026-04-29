#include"annealing.c"

int main(){
    const int sideSize = 6;
    squerVal out[sideSize*sideSize];
    generateMagicSquer_annealing(out, sideSize);
    return 0;
}