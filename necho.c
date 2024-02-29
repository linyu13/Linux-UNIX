#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
    int j = 0;
    for (j = 0; j < argc; j++) {
        printf("argv[%d] = %s", argv[j]);
    }
    exit(EXIT_SUCCESS);

    // 因为argv列表以NULL终止，所以可以将以上代码改写如下

    char **p;
    for (p = argv; *p != NULL; p++) {
        puts(*p);
    }
    exit(EXIT_SUCCESS);
}