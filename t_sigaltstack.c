#define _GUN_SOURCE
#include "tlpi_hdr.h"  // 包含头文件，定义了一些常用的宏和函数
#include <signal.h>    // 包含信号处理相关的头文件
#include <string.h>    // 包含字符串处理相关的头文件

// SIGSEGV 信号处理函数
static void sigsegvHandler(int sig) {
    int x;

    printf("Caught signal %d (%s)\n", sig, strsignal(sig)); // 打印捕获的信号和信号名称
    printf("Top of handler stack near      %10p\n", (void *)&x); // 打印信号处理函数栈的顶部地址
    fflush(NULL); // 刷新输出缓冲区

    _exit(EXIT_FAILURE); // 退出程序
}

// 递归函数，导致栈溢出
static void overflowStack(int callNum) {
    char a[10000]; // 定义一个大的局部数组

    printf("Call %4d - top of stack near %10p\n", callNum, &a[0]); // 打印函数调用信息
    overflowStack(callNum + 1); // 递归调用
}

int main(int argc, char *argv[]) {
    stack_t sigstack; // 定义一个信号处理栈结构体
    struct sigaction sa; // 定义一个信号处理结构体
    int j;

    printf("Top of standard stack is near %10p\n", (void *)&j); // 打印标准栈的顶部地址

    sigstack.ss_sp = malloc(SIGSTKSZ); // 分配备用信号栈的内存空间
    if (sigstack.ss_sp == NULL) {
        errExit("malloc"); // 如果分配失败，打印错误信息并退出程序
    }

    sigstack.ss_size = SIGSTKSZ; // 设置备用信号栈的大小
    sigstack.ss_flags = 0; // 清空备用信号栈的标志位
    if (sigaltstack(&sigstack, NULL) == -1) {
        errExit("sigaltstack"); // 如果设置备用信号栈失败，打印错误信息并退出程序
    }

    printf("Alternate stack is at           %10p-%p\n", sigstack.ss_sp,
           (char *)sbrk(0) - 1); // 打印备用信号栈的地址范围

    sa.sa_handler = sigsegvHandler; // 设置信号处理函数
    sigemptyset(&sa.sa_mask); // 清空信号掩码

    sa.sa_flags = SA_ONSTACK; // 设置 SA_ONSTACK 标志位
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        errExit("sigaction"); // 如果注册信号处理函数失败，打印错误信息并退出程序
    }

    overflowStack(1); // 调用递归函数，导致栈溢出
}
