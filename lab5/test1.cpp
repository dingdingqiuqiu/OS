#include <string.h>
#include <iostream>
#include <queue>
#include <set>

using namespace std;

struct PCB {
    string name;        // 进程名
    PCB* nxt;           // 指针，指向下一个进程
    int required_time;  // 要求运行时间
    int priority;       // 优先数
    string state;       // 状态
};

int main() {
    int cnt = 0;  // 标记运行次数
                  // 创建实例化对象
    PCB* p1 = new PCB;
    PCB* p2 = new PCB;
    PCB* p3 = new PCB;
    PCB* p4 = new PCB;
    PCB* p5 = new PCB;
    PCB* head = new PCB;
    head->nxt = p2;

    // 实例化对象赋值
    p1->name = "p1", p1->nxt = NULL;
    p1->required_time = 2, p1->priority = 1;
    p1->state = "R";

    p2->name = "p2", p2->nxt = p4;
    p2->required_time = 3, p2->priority = 5;
    p2->state = "R";

    p3->name = "p3", p3->nxt = p5;
    p3->required_time = 1, p3->priority = 3;
    p3->state = "R";

    p4->name = "p4", p4->nxt = p3;
    p4->required_time = 2, p4->priority = 4;
    p4->state = "R";

    p5->name = "p5", p5->nxt = p1;
    p5->required_time = 4, p5->priority = 2;
    p5->state = "R";

    while (head->nxt != NULL) {  // 当就绪队列不为空时
                                 // 从队列中取出优先级最大的进程
        PCB* curPCB = head->nxt;
        head->nxt = curPCB->nxt;

        // 打印当前运行的进程的名称
        cout << "当前运行的进程是" << curPCB->name << endl;

        // 当前进程优先级和运行时间减1
        curPCB->priority--;
        curPCB->required_time--;
        cnt++;

        // 如果需要运行时间不为0，将其加入到就绪队列
        if (curPCB->required_time > 0) {
            PCB* p = head;
            while (p->nxt != NULL && p->nxt->priority > curPCB->priority) {
                p = p->nxt;
            }

            curPCB->nxt = p->nxt;
            p->nxt = curPCB;
        }

        // 打印运行后的队列信息
        cout << "第" << cnt << "次运行后:" << endl;
        cout << "进程名\t下一个进程\t要求运行时间\t优先数\t状态" << endl;
        PCB* p = head;
        while (p->nxt != NULL) {
            cout << p->nxt->name << "\t";
            if (p->nxt->nxt != NULL)
                cout << p->nxt->nxt->name << "\t\t";
            else
                cout << "无\t\t";
            cout << p->nxt->required_time << "\t\t" << p->nxt->priority << "\t"
                 << p->nxt->state << endl;

            p = p->nxt;
        }

        cout << endl;
    }

    return 0;
}
