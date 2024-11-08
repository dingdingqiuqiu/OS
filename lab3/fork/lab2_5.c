#include <stdio.h>    // 包含 I/O 操作，如 printf()
#include <unistd.h>   // 包含 fork() 和 getpid() 等函数
#include <sys/wait.h> // 包含 wait() 函数

int main() {
    pid_t pid;  // 定义进程ID类型的变量

    // ① 创建子进程
    pid = fork();  

    // ② 判断当前是父进程还是子进程
    if (pid == 0) {  // 子进程执行的代码
        while (1) {
            printf("Child: pid=%d, ppid=%d\n", getpid(), getppid());  // 打印子进程的PID和父进程的PID
            sleep(3);
        }
    } else if (pid > 0) {  // 父进程执行的代码
        int i = 0;
        for (int i = 0; i < 10; i++) {
            printf("Parent: Child=%d, pid=%d, ppid=%d\n", pid, getpid(), getppid());  
            sleep(2);
        }
        printf("Father ends.\n");  // 父进程结束
    } else {
        // 如果 fork() 失败，输出错误信息
        perror("fork failed");
        return 1;  // 返回错误码
    }

    return 0;  // 正常退出程序
}

