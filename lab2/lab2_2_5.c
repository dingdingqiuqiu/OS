#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define MY_SHMKEY 10071800        // 共享内存键值
#define MY_SEMKEY 10071801        // 信号量键值
#define MAX_BLOCK 1024
#define NUM_CLIENTS 5             // 客户端进程数量

struct shmbuf {
    int top;
    int stack[MAX_BLOCK];
} *shmptr;

int shmid, semid;

void sigend(int);
void initialize_shared_memory();
void initialize_semaphore();
void sem_wait(int semid);
void sem_signal(int semid);
int getblock(int client_id);

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
    
    // 初始化信号量
    initialize_semaphore();

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
    semctl(semid, 0, IPC_RMID);  // 删除信号量
    return 0;
}

void sigend(int sig) {
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);  // 删除信号量
    exit(0);
}

void initialize_shared_memory() {
    shmptr->top = MAX_BLOCK - 1;
    for (int i = 0; i < MAX_BLOCK; i++)
        shmptr->stack[i] = MAX_BLOCK - i;
}

void initialize_semaphore() {
    // 创建信号量
    semid = semget(MY_SEMKEY, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("Failed to create semaphore");
        exit(1);
    }
    // 初始化信号量为1，表示可用
    if (semctl(semid, 0, SETVAL, 1) < 0) {
        perror("Failed to initialize semaphore");
        exit(1);
    }
}

void sem_wait(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;  // P操作
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) < 0) {
        perror("sem_wait failed");
    }
}

void sem_signal(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;  // V操作
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) < 0) {
        perror("sem_signal failed");
    }
}

int getblock(int client_id) {
    sem_wait(semid);  // 请求信号量

    if (shmptr->top < 0) {
        sem_signal(semid);  // 释放信号量
        return -1;  // 没有可用的块
    }
    
    // 模拟并发访问
    int block = shmptr->stack[shmptr->top];
    printf("Client %d is attempting to get block: %d (top is %d)\n", client_id, block, shmptr->top);
    shmptr->top--;  // 修改共享数据

    sem_signal(semid);  // 释放信号量
    return block;  // 成功获取块
}
