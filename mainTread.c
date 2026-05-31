#include <pthread.h>
#include <stdio.h>
#include"generatory/generatory.h"
#include<unistd.h>

typedef struct{pthread_t toKill; unsigned int after;} killAfter_arg;
void* killAfter(void* arg){
    killAfter_arg* parse_arg = arg;
    // sleep(parse_arg->after);
    printf("\n");
    for (int i = 0; i < parse_arg->after; i++){
        printf("\033[1A%5ds\n", i);
        sleep(1);
    }
    pthread_cancel(parse_arg->toKill);
    return NULL;
}

typedef struct{void (*gen)(squerVal*, uint32_t); uint32_t size;} generateSquer_arg;
void* generateSquer(void* arg){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    generateSquer_arg parse_arg = *(generateSquer_arg*)arg;
    squerVal out[parse_arg.size*parse_arg.size];
    parse_arg.gen(out, parse_arg.size);

    squerVal* outC = out;
    for(int y = 0; y < parse_arg.size; y++){
        for(int x = 0; x < parse_arg.size; x++) printf("%3d ", *(outC++));
        printf("\n");
    }

    return NULL;
}

void testGenerationSpeed(void (*gen)(squerVal*, uint32_t), unsigned int timeLimit){
    pthread_t thread_gen;
    pthread_t thread_limit;

    killAfter_arg thread_limit_arg = {.after = timeLimit};
    generateSquer_arg thread_gen_arg = {.gen = gen};

    // for(uint32_t size = 3; size<=20; size++){
    for(uint32_t size = 3; 1; size++){
        time_t start;
        time(&start);
        // clock_t start = clock();
        printf("size: %d\n", size);

        thread_gen_arg.size = size;
        pthread_create(&thread_gen, NULL, generateSquer, &thread_gen_arg);
        thread_limit_arg.toKill = thread_gen;
        pthread_create(&thread_limit, NULL, killAfter, &thread_limit_arg);

        void* retVal;
        pthread_join(thread_gen, &retVal);
        pthread_cancel(thread_limit);
        
        printf("\n");
        if(retVal == PTHREAD_CANCELED) break;
    }
    printf("\n");
}

int main() {
    int time = 600;
    // printf("generateMagicSquer_lexical\n");
    // testGenerationSpeed(generateMagicSquer_lexical, time);
    // printf("generateMagicSquer_force\n");
    // testGenerationSpeed(generateMagicSquer_force, time);
    // printf("generateMagicSquer_smartPermu\n");
    // testGenerationSpeed(generateMagicSquer_smartPermu, time);

    // printf("generateMagicSquer_annealing\n");
    // testGenerationSpeed(generateMagicSquer_annealing,time);
    // printf("generateMagicSquer_probForce\n");
    // testGenerationSpeed(generateMagicSquer_probForce,time);
    // printf("generateMagicSquer_setForce\n");
    // testGenerationSpeed(generateMagicSquer_probForce,time);
    printf("generateMagicSquer_evolve\n");
    testGenerationSpeed(generateMagicSquer_evolve,time);

    printf("end\n");
    return 0;
}