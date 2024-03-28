#include "tlpi_hdr.h"
#include <pthread.h>

static void* threadFunc(void* arg)
{
    char *s = (char *) arg;
    printf("%s", s);

    return (void *) strlen(s);
}


int main(int argc, char *argv[]) {
    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        perror("pthread_create");
    
    printf("Thread return %ld\n", (long) res);

    exit(EXIT_SUCCESS);
}