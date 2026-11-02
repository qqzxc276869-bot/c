#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	//1.值不能 修   地址可以修
	int b = 10;
	//const  int * ptr = &b;
	//int const * ptr1 = &b;
	//int a = 20;
	////*ptr = 100;  //值不能修
	//ptr = &a;  //地址可以修改
	//printf("%d", *ptr); //20

	////*ptr1 = 200;
	//ptr1 = &a;

	//2.  地址不能修  值可以修
	//int* const ptr = &b;
	//*ptr = 100;  //值可以修改
	//printf("%d  %d", *ptr, b); //100 100
	//int c = 200;
	//ptr = &c;  //地址不 能改

	//3. 值和址都不能修改
	const  int* const ptr = &b;
	/**ptr = 100;
	int d = 300;
	ptr = &d;*/
	return 0;
}

/*

const int a=100; // 定义常量      只读变量
//指针结合
int b=10;
const  int * ptr=&b;
int const * ptr=&b;


  int * const ptr=&b;

  const  int * const ptr=&b;

从左向右看，跳过数据类型，看const后跟的什么？
跟*  说明 值不能修改，跟的是 指针变量  说明地址不能修

*/

/*指针应用场景
1. 间接访问变量   和 访问连续的空间（一组数组 或二维数组）
2.数据结构中  链表  和 二叉树


*/