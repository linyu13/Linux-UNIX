#define _GUN_SOURCE
#include "signal_functions.h"  // 包含自定义的信号处理函数头文件
#include "tlpi_hdr.h"  // 包含自定义的头文件
#include <signal.h>  // 包含信号处理相关的标准头文件

static int sigCnt[NSIG];  // 统计各个信号的次数
static volatile sig_atomic_t gotSigint = 0;  // 标志变量，用于表示是否收到了 SIGINT 信号

static void handler(int sig) {  // 信号处理函数
    if (sig == SIGINT)
        gotSigint = 1;  // 如果收到了 SIGINT 信号，则设置标志变量
    else
        sigCnt[sig]++;  // 否则，增加相应信号的计数
}

int main(int argc, char *argv[]) {
    int n, numSecs;
    sigset_t pendingMask, blockingMask, emptyMask;

    printf("%s: PID id %ld\n", argv[0], (long)getpid());  // 输出当前进程的PID

    for (n = 1; n < NSIG; n++) {
        (void)signal(n, handler);  // 注册信号处理函数
    }

    if (argc > 1) {  // 如果有命令行参数
        numSecs = getInt(argv[1], GN_GT_o, NULL);  // 将第一个参数解析为等待的秒数

        sigfillset(&blockingMask);  // 将所有信号添加到阻塞集合中
        if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1) {  // 设置信号屏蔽字
            errExit("sigprocmask");
        }

        printf("%s sleeping for %d seconds\n", argv[0], numSecs);  // 输出等待消息
        sleep(numSecs);  // 等待指定秒数

        if (sigpending(&pendingMask) == -1) {  // 获取待处理信号集
            errExit("sigpending");
        }

        printf("%s pending signals are: \n", argv[0]);  // 输出待处理信号集
        printSigset(stdout, "\t\t", &pendingMask);

        sigemptyset(&emptyMask);  // 清空信号集
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1) {  // 解除信号屏蔽
            errExit("sigprocmask");
        }
    }

    while (!gotSigint)  // 等待收到 SIGINT 信号
        continue;

    for (n = 1; n < NSIG; n++) {  // 遍历所有信号
        if (sigCnt[n] != 0) {  // 如果有信号被捕获
            printf("%s: signal %d caught %d time%s\n", argv[0], n, sigCnt[n],
                   (sigCnt[n] == 1) ? "" : "s");  // 输出捕获的信号次数
        }
    }

    exit(EXIT_SUCCESS);
}
