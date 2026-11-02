#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int func()
{
    int f1=1, f2=1,f,sum=0;
    for (int i = 2; i < 40; i++)
    {
        f = f1 + f2;
        sum += f;
        f1 = f2;
        f2 = f;
    }
    printf("%d", f);
    return 0;
}
int main()
{
  /*  后第3个月起每个月都生一对兔子，小兔子长到第三问每个月的免子总数为多少 ? (输出前40个月即
        7.古典问题(兔子生崽) :有一对免子，从出生
        个月后每个月又生一对兔子，假如兔子都不死，
        可)。*/
    func();
}