#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    char *arglist[3];

    arglist[0] = "ls";
    arglist[1] = "-l";
    arglist[2] = 0;
    printf("1\n");
    execvp("ls", arglist);
    printf("2\n");
}