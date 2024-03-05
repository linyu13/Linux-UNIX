#define _GUN_SOURCE
#include "signal_functions.h"  // 包含自定义的信号处理函数头文件
#include "tlpi_hdr.h"  // 包含自定义的头文件
#include <signal.h>  // 包含信号处理相关的标准头文件
#include <string.h>  // 包含字符串处理相关的标准头文件

// 打印信号集
void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {  // 遍历所有可能的信号编号
        if (sigismember(sigset, sig)) {  // 检查信号是否在信号集中
            cnt++;
            // 输出信号编号、描述以及前缀
            fprintf(of, "%s%d(%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0) {
        fprintf(of, "%s<empty signal set>\n", prefix);  // 如果信号集为空，则输出空信号集
    }
}

// 打印当前进程的信号屏蔽字
int printSigMask(FILE *of, const char *msg) {
    sigset_t currMask;

    if (msg != NULL) {
        fprintf(of, "%s", msg);  // 如果有自定义的消息，则输出
    }

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1) {  // 获取当前进程的信号屏蔽字
        return -1;
    }

    printSigset(of, "\t\t", &currMask);  // 打印信号集

    return 0;
}

// 打印当前进程的待处理信号集
int printPeningSigs(FILE *of, const char *msg) {
    sigset_t pendingSigs;

    if (msg != NULL) {
        fprintf(of, "%s", msg);  // 如果有自定义的消息，则输出
    }

    if (sigpending(&pendingSigs) == -1) {  // 获取当前进程的待处理信号集
        return -1;
    }

    printSigset(of, "\t\t", &pendingSigs);  // 打印信号集

    return 0;
}
