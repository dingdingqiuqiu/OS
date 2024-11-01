#include <stdio.h>
#include "utils.h"

int main() {
    int a, b;
    printf("请输入待运算的两个值a和b:\n");
    scanf("%d %d",&a,&b);
    int tmp = yunsuan(a,b);
    printf("a和b的运算结果是%d\n",tmp);
    return 0;
}
