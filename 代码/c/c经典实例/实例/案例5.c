#define _CRT_SECURE_NO_WARNINGS	
#include<stdio.h>

int main1()
{
	/*题目：输入三个整数 x、y、z，请把这三个数由小到大输出。
		程序分析：我们想办法把最小的数放到 x 上，先将 x 与 y 进行比较，
		如果 x > y 则将 x 与 y 的值进行交换，然后再用 x 与 z 进行比较，
		如果 x > z 则将 x 与 z 的值进行交换，这样能使 x 最小。*/

	int x, y, z, temp;
	printf("请输入三个整数：");
	scanf("%d %d %d", &x, &y, &z);
	temp = x;
	if(x>y){
		x = y;
		y = temp;
	}
	if(x>z){
		x = z;
		z = temp;
	}
	if (y>z)
	{
		temp = y;
		y = z;
		z = temp;
	}
	printf("从小到大输出为：%d %d %d\n", x, y, z);

}


