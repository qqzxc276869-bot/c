#include<stdio.h>
int main()
{
	int year;
	printf("输入年份");
	scanf("%d",&year);
	if((year%4==0)&&(year%100!=0)||(year%400==0))
	{
		printf("是闰年");
	}
	else
	{
		printf("是平年"); 
	 } 

}


