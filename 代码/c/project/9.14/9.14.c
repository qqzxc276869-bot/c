#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main()
{
// 题目：有一分数序列：2/1，3/2，5/3，8/5，13/8，21/13...求出这个数列的前20项之和。
// 程序分析：请抓住分子与分母的变化规律。
    float sum1=0,sum2=0,sum=0;
    int a[20]={0},b[20]={0},count=0;
    a[0]=2,b[0]=1;
for (int i = 0; i < 20; i++)
{ count++;
    sum+=(double)a[i]/b[i];
    a[i+1]=a[i]+b[i];
    b[i+1]=a[i];
    printf("%.3lf ",sum);
}

}
