#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pid1, pid2;  // 子进程ID

// 子进程1的信号处理函数
void child1_handler(int sig) {
    printf("Child process 1 is killed by parent!!\n");
    exit(0);
}

// 子进程2的信号处理函数
void child2_handler(int sig) {
    printf("Child process 2 is killed by parent!!\n");
    exit(0);
}

void parent_handler(int sig) {
    printf("Parent sending termination signals to children...\n");
    
    // 向子进程发送信号 SIGUSR1 和 SIGUSR2
    if (pid1 > 0) kill(pid1, SIGUSR1);  // 向子进程1发送信号
    printf("hello\n");
    printf("%d,%d\n",pid1,pid2);
    if (pid2 > 0) kill(pid2, SIGUSR2);  // 向子进程2发送信号
    printf("Hello\n");
}

// 父进程的终止子进程的函数
void terminate_children() {
    printf("Parent sending termination signals to children...\n");
    
    // 向子进程发送信号 SIGUSR1 和 SIGUSR2
    if (pid1 > 0) kill(pid1, SIGUSR1);  // 向子进程1发送信号
    
    printf("%d,%d\n",pid1,pid2);
    if (pid2 > 0) kill(pid2, SIGUSR2);  // 向子进程2发送信号
}

int main() {
    // 创建子进程1
    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork child 1");
        exit(1);
    } else if (pid1 == 0) {
        // 子进程1设置信号处理器，并进入等待信号的状态
        signal(SIGUSR1, child1_handler);
        while (1) pause();  // 等待信号
    }

    // 创建子进程2
    pid2 = fork();
    if (pid2 < 0) {
        perror("Failed to fork child 2");
        exit(1);
    } else if (pid2 == 0) {
        // 子进程2设置信号处理器，并进入等待信号的状态
        signal(SIGUSR2, child2_handler);
        while (1) pause();  // 等待信号
    }

    // 父进程通过等待键盘输入来控制终止信号
    // printf("Press 'q' and Enter to terminate child processes:\n");
    // char input;
    // while (1) {
    //     input = getchar();
    //     if (input == 'q') {  // 输入 'q' 表示终止
    //         terminate_children();
    //         break;
    //     }
    // }
    signal(SIGTSTP, parent_handler);

    // 等待两个子进程终止
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent process is killed!!\n");
    return 0;
}
