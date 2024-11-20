#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child1, child2;  // 保存子进程的PID

// 子进程1的信号处理函数
void child1_handler(int sig) {
    printf("Child process 1 is killed by parent !!\n");
    exit(0);  // 终止子进程1
}

// 子进程2的信号处理函数
void child2_handler(int sig) {
    printf("Child process 2 is killed by parent !!\n");
    exit(0);  // 终止子进程2
}

// 父进程的信号处理函数
void parent_handler(int sig) {
    printf("Parent process received interrupt signal.\n");
    kill(child1, SIGUSR1);  // 向子进程1发送软中断信号SIGUSR1
    kill(child2, SIGUSR2);  // 向子进程2发送软中断信号SIGUSR2
}

int main() {
    // 创建子进程1
    child1 = fork();
    if (child1 < 0) {
        perror("fork failed for child1");
        exit(1);
    } else if (child1 == 0) {  // 子进程1执行的代码
        signal(SIGUSR1, child1_handler);  // 注册信号SIGUSR1的处理函数
        pause();  // 等待信号
    }

    // 创建子进程2
    child2 = fork();
    if (child2 < 0) {
        perror("fork failed for child2");
        exit(1);
    } else if (child2 == 0) {  // 子进程2执行的代码
        signal(SIGUSR2, child2_handler);  // 注册信号SIGUSR2的处理函数
        pause();  // 等待信号
    }

    // 父进程注册SIGTSTP信号处理函数
    signal(SIGTSTP, parent_handler);
    raise(SIGTSTP);

    // 父进程等待子进程终止
    wait(NULL);  // 等待任意子进程结束
    wait(NULL);  // 等待剩余的一个子进程结束
    printf("Parent process is killed!!\n");

    return 0;
}
