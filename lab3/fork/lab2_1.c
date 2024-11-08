#include <stdio.h>    // 包含 I/O 操作，如 printf()
#include <unistd.h>   // 包含 fork() 和 getpid() 等函数
#include <sys/wait.h> // 包含 wait() 函数

int main() {
    pid_t pid;  // 定义进程ID类型的变量

    // ① 创建子进程
    pid = fork();  

    // ② 判断当前是父进程还是子进程
    if (pid == 0) {  // 子进程执行的代码
        sleep(3);  // 子进程休眠3秒
        printf("Child: pid=%d, ppid=%d\n", getpid(), getppid());  // 打印子进程的PID和父进程的PID
    } 
    else if (pid > 0) {  // 父进程执行的代码
        printf("Parent: Child=%d, pid=%d, ppid=%d\n", pid, getpid(), getppid());  
        // 打印子进程的PID、父进程的PID和父进程的父进程的PID
        wait(NULL);  // 等待子进程结束
        printf("After Child ends.\n");  // 子进程结束后父进程继续执行
    } 
    else {
        // 如果 fork() 失败，输出错误信息
        perror("fork failed");
        return 1;  // 返回错误码
    }

    // ③ 父进程和子进程都会执行的代码
    printf("In which process?\n");

    return 0;  // 正常退出程序
}

