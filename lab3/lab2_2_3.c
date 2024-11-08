#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>

#define MY_SHMKEY 10071800		// need to change
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
int  getblock(void);
void showhelp(void);
void showlist(void);
void getcmdline(void);

int main(void)
{

    // 客户端运行
    if((shmid=shmget(MY_SHMKEY, sizeof(struct shmbuf), IPC_CREAT|IPC_EXCL|0666)) < 0)
    {/* shared memory exists, act as client */
        shmid=shmget(MY_SHMKEY, sizeof(struct shmbuf), 0666);
        shmptr=(struct shmbuf *)shmat(shmid, 0, 0);
	    local.top=-1;
	    showhelp();
	    getcmdline();
        while(strcmp(cmdbuf,"end\n"))
	    {
	        if(!strcmp(cmdbuf,"get\n"))
		        getblock();
	        else if(!strcmp(cmdbuf,"rel\n"))
		        relblock();
	        else if(!strcmp(cmdbuf,"list\n"))
		        showlist();
	        else if(!strcmp(cmdbuf,"help\n"))
		        showhelp();
	        getcmdline();
        }
    }
    else		/* 作为服务器运行 */
    {
	    int i;
        shmptr=(struct shmbuf *)shmat(shmid, 0, 0);
        signal(SIGINT, sigend);
        signal(SIGTERM, sigend);
        printf("NO OTHER OPERATION but press Ctrl+C or use kill to end.\n");
	    shmptr->top=MAX_BLOCK-1;
	    for(i=0; i<MAX_BLOCK; i++)
	        shmptr->stack[i]=MAX_BLOCK-i;
        sleep(1000000);	/* cause sleep forever. */ 
    }
}

void sigend(int sig)
{
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, IPC_RMID, 0);
    exit(0);
}

void relblock(void)
{
    if(local.top<0)
    {
		printf("No block to release!");
		return;
    }
    shmptr->top++;
    shmptr->stack[shmptr->top]=local.stack[local.top--];
    // printf("%d\n",local.top);
}

int getblock(void)
{
    if(shmptr->top<0)
    {
		printf("No free block to get!");
		return 0;
    }
    local.stack[++local.top]=shmptr->stack[shmptr->top];
    // printf("%d\n",local.top);
    shmptr->top--;
}

// 帮助信息
void showhelp(void)
{
    printf("\navailable COMMAND:\n\n");
    printf("help\tlist this help\n");
    printf("list\tlist all gotten block number\n");
    printf("get\tget a new block\n");
    printf("rel\trelease the last gotten block\n");
    printf("end\texit this program\n");
}

// 列出本地获得的块
void showlist(void)
{
    int i;
    printf("List all gotten block number:\n");
    for(i=0; i<=local.top; i++)
	    printf("%d\t", local.stack[i]);
}

// 读取命令
void getcmdline(void)
{
    printf("\n?> ");
    fgets(cmdbuf, MAX_CMD-1, stdin);
}

