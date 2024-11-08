#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define MY_SHMKEY 10071800        // 共享内存键值
#define MAX_BLOCK 1024
#define NUM_CLIENTS 5             // 客户端进程数量

struct shmbuf {
    int top;
    int stack[MAX_BLOCK];
} *shmptr;

void sigend(int);
int getblock(int client_id);
void initialize_shared_memory();

int shmid;

int main(void) {
    // 创建共享内存
    if ((shmid = shmget(MY_SHMKEY, sizeof(struct shmbuf), IPC_CREAT | 0666)) < 0) {
        perror("Failed to create shared memory");
        exit(1);
    }
    shmptr = (struct shmbuf *)shmat(shmid, NULL, 0);
    if (shmptr == (void *)-1) {
        perror("Failed to attach shared memory");
        exit(1);
    }

    // 初始化共享内存
    initialize_shared_memory();

    // 创建多个客户端进程
    for (int i = 0; i < NUM_CLIENTS; i++) {
        if (fork() == 0) {  // 子进程（客户端）
            srand(time(NULL) ^ (getpid() << 16)); // 随机种子
            for (int j = 0; j < 10; j++) {  // 每个客户端尝试获取10次
                sleep(rand() % 2);  // 随机等待，增加冲突可能性
                int block = getblock(i);
                if (block >= 0) {
                    printf("Client %d got block: %d\n", i, block);
                } else {
                    printf("Client %d failed to get block!\n", i);
                }
            }
            exit(0);
        }
    }

    // 作为服务器等待所有子进程完成
    for (int i = 0; i < NUM_CLIENTS; i++) {
        wait(NULL);
    }

    // 清理
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

void sigend(int sig) {
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

int getblock(int client_id) {
    int attempts = 0;  // 尝试次数
    while (attempts < 5) {  // 最大尝试次数
        if (shmptr->top < 0) {
            return -1;  // 没有可用的块
        }
        
        // 模拟并发访问
        int block = shmptr->stack[shmptr->top];
        printf("Client %d is attempting to get block: %d (top is %d)\n", client_id, block, shmptr->top);
        shmptr->top--;  // 这里没有锁，所以可能会发生冲突

        return block;  // 成功获取块
    }
    return -1;  // 达到最大尝试次数，返回失败
}

void initialize_shared_memory() {
    shmptr->top = MAX_BLOCK - 1;
    for (int i = 0; i < MAX_BLOCK; i++)
        shmptr->stack[i] = MAX_BLOCK - i;
}
