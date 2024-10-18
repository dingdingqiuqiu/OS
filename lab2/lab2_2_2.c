#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MY_SHMKEY 10071500
#define MY_SEMKEY 10071500

void sigend(int);
int shmid;
int main(void) {
    int *shmptr, local;

    if ((shmid = shmget(MY_SHMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        // 作为客户端运行, 客户端是生产者
        shmid = shmget(MY_SHMKEY, sizeof(int), 0666);
        shmptr = (int *)shmat(shmid, 0, 0);
        printf("Act as producer. To end, input 0 when prompted.\n\n");
        // printf("Input a number:\n");
        // scanf("%d", &local);
        local = 1;
        while (local) {
            // 注释掉信号量操作，模拟数据竞争
            // semopbuf.sem_num = 0;
            // semopbuf.sem_op = -1;
            // semopbuf.sem_flg = SEM_UNDO;
            // semop(semid, &semopbuf, 1); /* P(S1) */

            *shmptr = local; // 直接写入共享内存

            // semopbuf.sem_num = 1;
            // semopbuf.sem_op = 1;
            // semopbuf.sem_flg = SEM_UNDO;
            // semop(semid, &semopbuf, 1); /* V(S2) */

            local++; 
            // printf("Input a number:\n");
            // scanf("%d", &local);
        }
    } else {
        // 作为服务器运行
        shmptr = (int *)shmat(shmid, 0, 0);
        signal(SIGINT, sigend); // 捕捉 Ctrl+C 信号
        signal(SIGTERM, sigend); // 捕捉终止信号
        printf("ACT CONSUMER!!! To end, try Ctrl+C or use kill.\n\n");

        while (1) {
            // 注释掉信号量操作，模拟数据竞争
            // semopbuf.sem_num = 1;
            // semopbuf.sem_op = -1;
            // semopbuf.sem_flg = SEM_UNDO;
            // semop(semid, &semopbuf, 1); /* P(S2) */

            printf("Shared memory set to %d\n", *shmptr);

            // semopbuf.sem_num = 0;
            // semopbuf.sem_op = 1;
            // semopbuf.sem_flg = SEM_UNDO;
            // semop(semid, &semopbuf, 1); /* V(S1) */
        }
    }
}

void sigend(int sig) {
    shmctl(shmid, IPC_RMID, 0); // 删除共享内存段
    // semctl(semid, IPC_RMID, 0); // 删除信号量
    exit(0); // 结束进程
}

