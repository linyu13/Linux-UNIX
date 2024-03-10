#define _GUN_SOURCE
#include "signal_functions.h" // 包含自定义的信号处理函数
#include "tlpi_hdr.h" // 包含头文件，定义了一些常用的宏和函数
#include <setjmp.h>   // 包含信号跳转相关的头文件
#include <signal.h>   // 包含信号处理相关的头文件
#include <string.h>   // 包含字符串处理相关的头文件

static volatile sig_atomic_t canJump = 0; // 定义一个用于信号跳转的标志变量

#ifdef USE_SIGSETJMP // 根据编译器标志选择信号跳转的方式
static sigjmp_buf senv; // 使用 sigjmp_buf 类型的变量保存信号跳转的上下文
#else
static jmp_buf env; // 使用 jmp_buf 类型的变量保存信号跳转的上下文
#endif

static void handler(int sig) {
    printf("Received signal %d (%s), signal mask is: \n", sig,
           strsignal(sig)); // 打印接收到的信号和信号名称

    printSigMask(stdout, NULL); // 打印当前信号掩码

    if (!canJump) { // 检查是否可以进行信号跳转
        printf(
            "'env' buffer not yet set, doing a simple return\n"); // 如果不能，打印提示信息
        return;
    }

#ifdef USE_SIGSETJMP     // 根据编译器标志选择信号跳转的方式
    siglongjmp(senv, 1); // 使用 siglongjmp 进行信号跳转
#else
    longjmp(env, 1); // 使用 longjmp 进行信号跳转
#endif
}

int main(int argc, char *argv[]) {
    struct sigaction sa; // 定义一个信号处理结构体

    printSigMask(stdout,
                 "Signal mask at starup:\n"); // 打印程序启动时的信号掩码

    sigemptyset(&sa.sa_mask); // 初始化信号处理结构体
    sa.sa_flags = 0;
    sa.sa_handler = handler;                // 设置信号处理函数
    if (sigaction(SIGINT, &sa, NULL) == -1) // 注册信号处理函数
        errExit("sigaction"); // 如果注册失败，打印错误信息并退出程序

#ifdef USE_SIGSETJMP // 根据编译器标志选择信号跳转的方式
    printf("Calling sigsetjmp()\n"); // 打印提示信息
    if (sigsetjmp(senv, 1) == 0)     // 设置信号跳转点
#else
    printf("Calling setjmp()\n"); // 打印提示信息
    if (setjmp(env) == 0)         // 设置信号跳转点
#endif
        canJump = 1; // 设置信号跳转标志

    else
        printSigMask(
            stdout,
            "After jump from handler, signal mask is:\n"); // 打印信号跳转后的信号掩码

    for (;;)
        pause(); // 等待信号
}