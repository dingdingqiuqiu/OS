#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int fd[2];          // 管道描述符数组
    pid_t pid1, pid2;   // 两个子进程ID
    char buf[50];       // 缓冲区，用于读取管道数据

    // 创建管道
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // 创建第一个子进程
    if ((pid1 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {  // 第一个子进程代码
        // 关闭读端，子进程只需要写入数据
        close(fd[0]);

        // 加锁，防止多个子进程同时写入
        lockf(fd[1], F_LOCK, 0);
        write(fd[1], "Child1 is sending a message!\n", strlen("Child1 is sending a message!\n"));
        lockf(fd[1], F_ULOCK, 0);  // 解锁

        close(fd[1]);  // 关闭写端
        exit(0);
    }

    // 创建第二个子进程
    if ((pid2 = fork()) == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) {  // 第二个子进程代码
        // 关闭读端，子进程只需要写入数据
        close(fd[0]);

        // 加锁，防止多个子进程同时写入
        lockf(fd[1], F_LOCK, 0);
        write(fd[1], "Child2 is sending a message!\n", strlen("Child2 is sending a message!\n"));
        lockf(fd[1], F_ULOCK, 0);  // 解锁

        close(fd[1]);  // 关闭写端
        exit(0);
    }

    // 父进程代码
    // 关闭写端，父进程只需要读取数据
    close(fd[1]);

    // 循环读取管道中的数据并打印到屏幕
    while (read(fd[0], buf, sizeof(buf) - 1) > 0) {
        buf[sizeof(buf) - 1] = '\0';  // 确保字符串以 '\0' 结尾
        printf("%s", buf);
    }

    // 关闭读端
    close(fd[0]);

    // 等待两个子进程结束
    wait(NULL);
    wait(NULL);

    return 0;
}
