#define _CRT_SECURE_NO_WARNINGS	
#include<stdio.h>

int main()
{
	/*输入某年某月某日，判断这一天是这一年的第几天？
		程序分析：以3月5日为例，应该先把前两个月的加起来，然后再加上5天即本年的第几天，
		特殊情况，闰年且输入月份大于3时需考虑多加一天*/
	int	year, month, day,sum_day=0,month_day[] = { 31,28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	printf("请输入年月日（格式：yyyy-mm-dd）：");
	scanf("%d %d %d", &year, &month, &day);
	if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
	{
			month_day[1] = 29;
	}
	for (int i = 0; i < month-1; i++)
	{
		sum_day += month_day[i];
	}
	printf("这是这一年的第%d天\n", sum_day+day);	
	

}


