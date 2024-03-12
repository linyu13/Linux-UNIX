#include <stdio.h>

int main() {
    printf("My pid is %d\n", getpid());
    fork();
    fork();
    fork();
    printf("my pid is %d\n", getpid());
}