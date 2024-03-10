#define _XOPEN_SOURCE 600
#include "tlpi_hdr.h"  // 包含自定义的头文件，提供了一些常用的函数声明和定义
#include <signal.h>    // 包含信号处理相关的头文件
#include <string.h>    // 包含字符串处理相关的头文件
#include <unistd.h>    // 包含用于系统调用的头文件

static char *str2;    // 全局变量，用于存储命令行参数中的第二个字符串
static int handled = 0;  // 全局变量，用于记录信号处理器执行次数

// 信号处理器函数
static void handler(int sig) {
    crypt(str2, "xx");  // 对命令行参数中的第二个字符串进行加密
    handled++;  // 更新信号处理器执行次数
}

int main(int argc, char *argv[]) {
    char *cr1;  // 存储命令行参数中的第一个字符串加密后的结果
    int callNum, mismatch;  // 用于记录循环执行次数和加密结果不匹配的次数
    struct sigaction sa;  // 定义信号处理结构体变量

    // 检查命令行参数是否合法
    if (argc != 3) {
        usageErr("%s str1 str2\n", argv[0]);  // 提示用户正确的用法
    }

    str2 = argv[2];  // 将命令行参数中的第二个字符串赋值给全局变量
    cr1 = strdup(crypt(argv[1], "xx"));  // 对命令行参数中的第一个字符串进行加密，并将结果复制给局部变量

    // 检查内存分配是否成功
    if (cr1 == NULL) {
        errExit("strdup");  // 如果内存分配失败，则退出程序并打印错误消息
    }

    // 设置信号处理器的行为
    sigemptyset(&sa.sa_mask);  // 清空信号屏蔽字
    sa.sa_flags = 0;  // 设置标志位为0，表示没有特殊行为
    sa.sa_handler = handler;  // 设置信号处理器函数为自定义的 handler 函数
    // 将 SIGINT 信号的处理器设置为 sa
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        errExit("sigaction");  // 如果设置信号处理器失败，则退出程序并打印错误消息
    }

    // 进入无限循环，直到手动中断程序
    for (callNum = 1, mismatch = 0;; callNum++) {
        // 比较加密结果是否与之前保存的结果相同
        if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
            mismatch++;  // 如果不同，则增加不匹配计数
            // 打印不匹配的信息，以及循环执行次数和信号处理器执行次数
            printf("Mismatch on call %d (mismatch = %d handled = %d)\n",
                   callNum, mismatch, handled);
        }
    }
}
