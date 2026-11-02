#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>  

int main()
{
	/*题目：两个乒乓球队进行比赛，各出三人，甲队为 a、b、c 三人，乙队为 x、y、z 三人。
		抽签决定比赛名单，有人向队员打听比赛的名单：a 说他不和 x 比，
		c 说他不和 x、z 比，请编写代码找出三队赛手的名单。
		思路：
		使用 3 个循环 分别枚举甲队的每个队员与乙队的队员匹配的可能性。
		添加 约束条件，过滤掉不满足题目要求的组合：
		a 不和 x 比赛。
		c 不和 x 和 z 比赛。
		确保每个乙队的队员只能和一个甲队的队员比赛。*/
	char A[3] = { 'a', 'b', 'c' }, B[3] = { 'x', 'y', 'z' };
	for (int i = 'x'; i <='z'; i++)
	{
		for (int j = 'x'; j <= 'z'; j++)
		{
			if (i!= j)
			{

			
				for (int k = 'x'; k <= 'z'; k++)
				{
					if ( i != k && j != k && i != 'x' && k != 'x' && k != 'z')
					{
					printf("a vs %c\n", i);
					printf("b vs %c\n", j);
					printf("c vs %c\n", k);
					}
				}
			}
		}

	}


}