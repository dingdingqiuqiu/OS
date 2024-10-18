// fork() 创建一个子进程，通过返回值区分在父进程还是子进程中执行；
// wait() 等待子进程执行完成
// sleep() 导致调用进程睡眠指定秒数
// getpid() 获取当前进程id
// 鼠标移动到函数上,按<shift>+k可以查看说明 
// getppid() 获取父进程id 
#include <sys/types.h> // 包含数据类型(如pid_t)
#include <unistd.h>     // 包含fork()等POSIX函数
#include <stdio.h>      // IO函数
#include <sys/wait.h>   // 包含wait()函数，用于等待子进程

int main(void) {
    int x1 = fork(); // 创建第一个子进程
    if (x1 == 0) {
        // 子进程1
        while(1) {
            printf("b\n");
            // printf("进程号为%d的子进程1打印b\n",getpid());
            // printf("进程号为%d的子进程1打印b,其父进程为%d\n",getpid(),getppid());
            sleep(1);
        }
    } else { // 只有父进程能创建第二个子进程
        int x2 = fork();
        if (x2 == 0) {
            // 子进程2
            while(1) {
                printf("c\n");
                // printf("进程号为%d的子进程2打印c\n",getpid());
                // printf("进程号为%d的子进程2打印c,其父进程为%d\n",getpid(),getppid());
                sleep(1);
            }
        } else {
            // 父进程
            while(1) {
                printf("a\n");
                // printf("进程号为%d的父进程打印a\n",getpid());
                // printf("进程号为%d的父进程打印a\n",getpid());
                sleep(1);
            }
        }
    }
    return 0;
}
