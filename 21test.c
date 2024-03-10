#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int counter = 0; // 全局计数器

// 信号处理器函数
void sigHandler(int sig) {
    printf("Signal handler called\n");
    counter++; // 更新计数器
}

int main() {
    signal(SIGINT, sigHandler); // 注册信号处理器函数

    while (1) {
        printf("Counter: %d\n", counter); // 打印计数器值
        sleep(1);                         // 主程序睡眠一秒钟
    }

    return 0;
}
