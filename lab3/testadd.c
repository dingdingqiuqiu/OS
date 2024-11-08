#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024  // 定义共享内存大小
#define SHM_KEY 1234   // 定义共享内存的键值

int main(int argc, char *argv[])
{
    int shmid;
    int *prt;

    // 创建共享内存
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    // 附加共享内存
    prt = (int *)shmat(shmid, NULL, 0);
    if (prt == (int *)(-1)) {
        perror("shmat failed");
        return 1;
    }

    int local = 0; // 你想要写入的值
    for (int i = 0;i < 0x8fff;i++) {
        *prt = local;       // 将值写入共享内存
        local++;
    }
    unsigned int x = 0x8fff;
    printf("%d\n",x);
    printf("Value in shared memory: %d\n", *prt); // 打印共享内存中的值

    // 解除共享内存的附加
    if (shmdt(prt) == -1) {
        perror("shmdt failed");
        return 1;
    }

    return 0;
}
