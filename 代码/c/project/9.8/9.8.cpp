#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "func.h"
int main()
{
	/*判断 101 到 200 之间的素数。

		程序分析：判断素数的方法：用一个数分别去除 2 到 sqrt(这个数)，
		如果能被整除，则表明此数不是素数，反之是素数。*/
	printf("100到200之间的素数：\n");
	func();


}