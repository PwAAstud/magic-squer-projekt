#include <stdio.h>
#include"generatory/generatory.h"
#include<time.h>

#ifdef _WIN32
#include<windows.h>
#elif unix
#include<unistd.h>
#endif

void generateSquer(void (*gen)(squerVal*, uint32_t), uint32_t size){
    squerVal out[size*size];
    gen(out, size);

    squerVal* outC = out;
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++) printf("%3d ", *(outC++));
        printf("\n");
    }
}

void testGenerationSpeed(void (*gen)(squerVal*, uint32_t), unsigned int timeLimit){
    for(uint32_t size = 3; 1; size++){
        time_t start;
        time(&start);

        generateSquer(gen, size);
                
        time_t end;
        time(&end);
        printf("size: %d; time %ld\n", size, end-start);
        // break;
        if(end-start > timeLimit) break;
    }
    printf("\n");
}

int main() {
    // printf("generateMagicSquer_lexical\n");
    // testGenerationSpeed(generateMagicSquer_lexical,60);
    printf("generateMagicSquer_force\n");
    testGenerationSpeed(generateMagicSquer_force, 600);
    // printf("generateMagicSquer_smartPermu\n");
    // testGenerationSpeed(generateMagicSquer_smartPermu, 9999999);

// #include"./probForce/probForce.h"
// #include"./setForce/setForce.h"
    printf("generateMagicSquer_annealing\n");
    testGenerationSpeed(generateMagicSquer_annealing,600);
    printf("generateMagicSquer_probForce\n");
    testGenerationSpeed(generateMagicSquer_probForce,600);
    printf("generateMagicSquer_setForce\n");
    testGenerationSpeed(generateMagicSquer_probForce,600);



    printf("end\n");
    return 0;
}