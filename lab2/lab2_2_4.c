#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>

#define MY_SHMKEY 10071800
#define MAX_BLOCK 1024
#define MAX_CMD 8

struct shmbuf {
    int top;
    int stack[MAX_BLOCK];
} *shmptr, local;

char cmdbuf[MAX_CMD];
int shmid, semid;

void sigend(int);
void relblock(void);
int getblock(void);
void showhelp(void);
void showlist(void);
void getcmdline(void);
void P(int semid);
void V(int semid);

int main(void) {
    // Create shared memory
    shmid = shmget(MY_SHMKEY, sizeof(struct shmbuf), IPC_CREAT | 0666);
    // Create semaphore
    semid = semget(MY_SHMKEY, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1); // Initialize semaphore to 1

    shmptr = (struct shmbuf *)shmat(shmid, 0, 0);
    local.top = -1;

    if (shmid == -1 || shmptr == (void *)-1) {
        perror("shmget or shmat failed");
        exit(1);
    }

    showhelp();
    getcmdline();
    while (strcmp(cmdbuf, "end\n")) {
        if (!strcmp(cmdbuf, "get\n"))
            getblock();
        else if (!strcmp(cmdbuf, "rel\n"))
            relblock();
        else if (!strcmp(cmdbuf, "list\n"))
            showlist();
        else if (!strcmp(cmdbuf, "help\n"))
            showhelp();
        getcmdline();
    }

    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    return 0;
}

void sigend(int sig) {
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, IPC_RMID, 0);
    exit(0);
}

void P(int semid) {
    struct sembuf sb = {0, -1, 0}; // Wait operation
    semop(semid, &sb, 1);
}

void V(int semid) {
    struct sembuf sb = {0, 1, 0}; // Signal operation
    semop(semid, &sb, 1);
}

void relblock(void) {
    P(semid); // Lock
    if (local.top < 0) {
        printf("No block to release!\n");
        V(semid); // Unlock
        return;
    }
    shmptr->top++;
    shmptr->stack[shmptr->top] = local.stack[local.top--];
    V(semid); // Unlock
}

int getblock(void) {
    P(semid); // Lock
    if (shmptr->top < 0) {
        printf("No free block to get!\n");
        V(semid); // Unlock
        return 0;
    }
    local.stack[++local.top] = shmptr->stack[shmptr->top];
    shmptr->top--;
    V(semid); // Unlock
}

void showhelp(void) {
    printf("\navailable COMMAND:\n\n");
    printf("help\tlist this help\n");
    printf("list\tlist all gotten block number\n");
    printf("get\tget a new block\n");
    printf("rel\trelease the last gotten block\n");
    printf("end\texit this program\n");
}

void showlist(void) {
    int i;
    printf("List all gotten block number:\n");
    for (i = 0; i <= local.top; i++)
        printf("%d\t", local.stack[i]);
    printf("\n");
}

void getcmdline(void) {
    printf("\n?> ");
    fgets(cmdbuf, MAX_CMD - 1, stdin);
}
