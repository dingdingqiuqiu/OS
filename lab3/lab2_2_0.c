#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MY_SHMKEY 10071500		// need to change
#define MY_SEMKEY 10071500		// need to change

void sigend(int);

int shmid, semid;

int main(void)
{
    int *shmptr, semval, local;
    struct sembuf semopbuf;

    if((shmid=shmget(MY_SHMKEY, sizeof(int), IPC_CREAT|IPC_EXCL|0666)) < 0)
    { /* 如果存在共享内存，作为客户端运行,客户端是生产者 */
        shmid=shmget(MY_SHMKEY, sizeof(int), 0666);
        semid=semget(MY_SEMKEY, 2, 0666);
        shmptr=(int *)shmat(shmid, 0, 0);
        printf("Act as producer. To end, input 0 when prompted.\n\n");
	    printf("Input a number:\n");
	    scanf("%d", &local);
        while( local )
	    {
	        semopbuf.sem_num=0;
	        semopbuf.sem_op=-1;
	        semopbuf.sem_flg=SEM_UNDO;
	        semop(semid, &semopbuf, 1);	/* P(S1) */
	        *shmptr = local;
	        semopbuf.sem_num=1;
	        semopbuf.sem_op=1;
	        semopbuf.sem_flg=SEM_UNDO;
	        semop(semid, &semopbuf, 1);	/* V(S2) */
	        printf("Input a number:\n");
	        scanf("%d", &local);
        }
    }
    else		/* acts as server */
    {
        semid=semget(MY_SEMKEY, 2, IPC_CREAT|0666);
        shmptr=(int *)shmat(shmid, 0, 0);
	    semval=1;
	    semctl(semid, 0, SETVAL, semval);	/* set S1=1 */
	    semval=0;
	    semctl(semid, 1, SETVAL, semval);	/* set S2=0 */
        signal(SIGINT, sigend);
        signal(SIGTERM, sigend);
        printf("ACT CONSUMER!!! To end, try Ctrl+C or use kill.\n\n");
        while(1)
        {
	        semopbuf.sem_num=1;
	        semopbuf.sem_op=-1;
	        semopbuf.sem_flg=SEM_UNDO;
	        semop(semid, &semopbuf, 1);	/* P(S2) */
            printf("Shared memory set to %d\n", *shmptr);
	        semopbuf.sem_num=0;
	        semopbuf.sem_op=1;
	        semopbuf.sem_flg=SEM_UNDO;
	        semop(semid, &semopbuf, 1);	/* V(S1) */
        }
    }
}

void sigend(int sig)
{
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, IPC_RMID, 0);
    exit(0);
}

