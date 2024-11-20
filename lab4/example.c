#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>  // 包含wait()函数
#include <string.h>    // 包含sprintf()等函数

int main() {
    int x, fd[2];
    char buf[30], s[30];

    // 创建管道
    pipe(fd);

    // 创建子进程
    while ((x = fork()) == -1);  // 如果创建失败，尝试再次fork

    if (x == 0) {
        // 子进程部分
        sprintf(buf, "this is an example\n");  // 向buf写入字符串
        printf("Child process running!\n");

        // 将buf中的数据写入管道
        write(fd[1], buf, 30);  // 使用strlen(buf)+1来确保包括'\0'

        // 睡眠5秒，给父进程时间来读取数据
        sleep(5);

        // 子进程退出
        exit(0);
    } else {
        // 父进程部分
        wait(NULL);  // 等待子进程结束

        // 从管道读取数据
        read(fd[0], s, 30);  // 从管道中读取数据到s数组

        // 输出从管道读取的数据
        printf("%s", s);
    }

    return 0;
}
