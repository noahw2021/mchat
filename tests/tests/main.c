#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread(void* a) {
    usleep(1000000);
    char A = 'a';
    ioctl(0, TIOCSTI, &A);
    return NULL;
}

int main(int argc, char** argv) {
    char Bfr[24];
    printf("abc: ");
    
    pthread_t thread2;
    pthread_create(&thread2, NULL, thread, NULL);
    
    fgets(Bfr, 24, stdi n);
    printf(" = %s\n", Bfr);
    
    return 0;
}
