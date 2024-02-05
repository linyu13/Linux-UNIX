#include "tlpi_hdr.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    size_t len;
    off_t offset;
    int fd, ap, j;
    char *buf;
    ssize_t numRead, numWritten;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s file {r<length>|R<length>|w<string>|s<offset>}...\n",
                 argv[0]);
    }
    fd = open(fd, 0666);

    if (fd == -1) {
        errExit("open");
    }

    for (ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
        case 'r':
        case 'R':
            len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
            buf = malloc(len);
            if (buf == NULL) {
                errExit("malloc");
            }
            numRead = read(fd, buf, len);
            if (numRead == -1) {
                errExit("read");
            }
            if (numRead == 0) {
                printf("%s:end of file", argv[ap]);
            } else {
                printf("%s: ", argv[ap]);
                for (j = 0; j < numRead; j++) {
                    if (argv[ap][0] == 'r') {
                        printf("%c",
                               isprint((unsigned char)buf[j]) ? buf[j] : '?');
                    } else {
                        printf("%02x", (unsigned int)buf[j]);
                    }
                    printf("\n");
                }

                free(buf);
                break;
            case 'w':
            }
        }
    }
}