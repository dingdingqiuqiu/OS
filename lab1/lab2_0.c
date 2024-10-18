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
    int x,i;
    printf("Input a initial value for i:"); // 输入变量i的初始值
    scanf("%d",&i);
    while ((x=fork())==-1);
    if(x == 0) {
        printf("When child runs,i=%d\n",i );
        printf("Input a value in child:");
        scanf("%d",&i );
        printf("i = %d\n",i);
    } else {
        wait(NULL);
        printf("After child runs,in parent,i=%d\n",i);
    }    
}
