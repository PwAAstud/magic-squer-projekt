#include <pthread.h>
#include <stdio.h>

#ifdef _WIN32
#include<windows.h>
#elif unix
#include<unistd.h>
#endif

void* ft1(void* arg) {
    printf("Created a new thread 1\n");
    sleep(10);
    printf("a\n");
    return NULL;
}

void* ft2(void* arg) {
    printf("Created a new thread 2\n");
    sleep(2);
    pthread_cancel( *(pthread_t*)arg );
    printf("b\n");
    return NULL;
}

int main() {
    
    // Create a pthread_t variable to store
    // thread ID
    pthread_t thread1;
    pthread_t thread2;
    
    time_t time1;
    time(&time1);
    
    // Creating a new thread. 
    pthread_create(&thread1, NULL, ft1, NULL);
    pthread_create(&thread2, NULL, ft2, &thread1);
    pthread_join(thread1, NULL);
    // printf("%d\n",pthread_cancel( thread1 ));
    // pthread_join(thread1, NULL);
    // sleep(2);
    // printf("%d\n",pthread_cancel( thread1 ));
    // sleep(10);
    
    time_t time2;
    time(&time2);
    printf("time %ld\n", time2-time1);

    return 0;
}