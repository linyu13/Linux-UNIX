#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env; // 定义了一个jmp_buf类型的全局变量env，用于保存跳转位置的信息

static void doJump(int nvar, int rvar, int vvar) {
    printf("Inside doJump(): nvar = %d rvar = %d vvar = %d\n", nvar, rvar,
           vvar);
    longjmp(env, 1); // 调用longjmp将程序跳转到setjmp处并传递参数1
}

int main(int argc, char *argv[]) {
    int nvar; // 自动变量
    register int rvar; // 寄存器变量
    volatile int vvar; // volatile变量

    nvar = 111; // 初始化自动变量
    rvar = 222; // 初始化寄存器变量
    vvar = 333; // 初始化volatile变量

    if (setjmp(env) == 0) { // 使用setjmp函数保存当前执行点的状态，并将env作为跳转目标的标识
        nvar = 777; // 修改自动变量
        rvar = 888; // 修改寄存器变量
        vvar = 999; // 修改volatile变量
        doJump(nvar, rvar, vvar); // 调用doJump函数
    } else { // 当从longjmp跳转回来时
        printf("After longjmp(): nvar = %d rvar = %d vvar = %d\n", nvar, rvar,
               vvar); // 打印各个变量的值
    }

    exit(EXIT_SUCCESS); // 退出程序
}
