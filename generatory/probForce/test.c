#include"probForce.c"

int main(){
    const int sideSize = 6;
    squerVal out[sideSize*sideSize];
    generateMagicSquer_probForce(out, sideSize);
}