#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
int main() {
	/*10.判断以下变量的命名不正确的是：A、  abc   B、 _abc   C、  a_bc   D、 #abc
	(要求编制一个程序用 switch语句，相当于选择题)*/
	char choose; 
	printf("判断以下变量的命名不正确的是：\n");
	printf("A. abc    B. _abc    C. a_bc    D. #abc\n");
	printf("请输入选项(A、B、C、D)：");
	scanf("%c", &choose);
	switch (choose)
	{
	case 'A':
	case 'a': printf("回答错误,选项A是正确变量名\n"); break;
	case 'B':
	case 'b': printf("回答错误,选项B是正确变量名\n"); break;
	case 'C':
	case 'c': printf("回答错误,选项C是正确变量名\n"); break;
	case 'D':
	case 'd': printf("回答正确,选项D是错误变量名\n"); break;
	
	}
}