#include "tlpi_hdr.h"
#include <setjmp.h>

static jmp_buf env; // 定义一个jmp_buf类型的全局变量env，用于保存跳转位置的信息

static void f2(void) { longjmp(env, 2); } // 定义一个函数f2，调用longjmp将程序跳转到setjmp处并传递参数2

static void f1(int argc) {
    if (argc == 1) {
        longjmp(env, 1); // 如果argc为1，则调用longjmp将程序跳转到setjmp处并传递参数1
    }
    f2(); // 否则调用函数f2()
}

int main(int argc, char *argv[]) {
    switch (setjmp(env)) { // 使用setjmp函数保存当前执行点的状态，并将env作为跳转目标的标识
    case 0: // 初始状态
        printf("Calling f1() after initial setjmp()\n"); // 打印消息
        f1(argc); // 调用f1函数
        break;

    case 1: // 当从f1中longjmp回来时
        printf("We jumped back from f1()"); // 打印消息
        break;

    case 2: // 当从f2中longjmp回来时
        printf("We jumped back from f2()"); // 打印消息
        break;
    }

    exit(EXIT_SUCCESS); // 退出程序
}
