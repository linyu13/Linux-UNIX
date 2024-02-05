#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s old-file new-file\n", argv[0]);
    }
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        errExit("open file is %s", argv[1]);
    }
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    /*
    这个组合通常用于打开一个文件，如果文件不存在，则创建它；
    如果文件已经存在，则清空文件内容，使文件变为空文件。
    这在某些情况下用于写入新的数据到文件中，或者重新初始化一个文件。
    */
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
                S_IWOTH; // 0666  rw-rw-rw-  r为4,w为2,x为1
                         /*
                         这个组合表示一个文件的默认权限，其中文件所有者具有读和写权限，
                         组具有读和写权限，其他用户也具有读和写权限。
                         这种权限设置通常用于允许文件的所有用户进行读写操作。
                         */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        errExit("opening file is %s", argv[2]);
    }
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            fatal("couldn't write whole buffer");
        }
    }
    if (numRead == -1) {
        errExit("read");
    }
    if (close(inputFd) == -1) {
        errExit("close input");
    }
    if (close(outputFd) == -1) {
        errExit("close output");
    }
    exit(EXIT_SUCCESS);
}