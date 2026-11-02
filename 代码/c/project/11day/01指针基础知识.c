#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	int num = 10;
   //直接访问    查看数据    修改数据
	num = 20;
	printf("num=%d\n", num);

	//间接访问      不直接操作num  通过指针的方法修改num的值  查看num的值       pointer
	int* ptr = &num;

	printf("%p  %p\n", ptr, &num);//00000050BDFCFB14  00000050BDFCFB14

	*ptr = 100;
	printf("%d  %d\n", num,*ptr);//100   100

	//------------------- 定义
	int  a = 200;
	int* p;  //读作  指向int类型空间的指针            int型指针
	p = &a;
	printf("%p  %p   %d  %d\n", p, &a, *p, a);
	printf("%p\n", &p);
	printf("%zu\n", sizeof(int*));

	/*
	000000903DAFF814  000000903DAFF814   200  200
     000000903DAFF838
    8         或  4
	*/

//注意：指针变量  只能地址 ，不能直接存值，   指针同类型的空间
	//int* pt = 100; //“int *”与“int”的间接级别不同
	int b = 20;
	double* ptr1 = &b;//从“int *”到“double *”的类型不兼容

	int c, d;
	//int* c1, d1;  //c1是指针变量 d1是整型变量
	int* c1, * d1;  //c1 d1是指针变量
	return 0;
}

/*
一、指针理解
   1. 变量的访问方式  两种
      直接访问
	  间接访问

	2.指针的概念
	  指针（pointer）：它是一种特殊的变量数据类型，它是用来存储内存地址的
	
	 指针变量：是存某个内存空间的地址的变量 ，简单说，指针变量直接说成指针
	目的： 通过访问指针变量达到访问内存中另一个变量数据的目的。    （间接访问）

二、指针定义语法

   数据类型   *  指针变量名[=初始地址值 ]

   int* ptr;                  
   int  *ptr;
   int * ptr;
   //* 修饰 ptr   说明 ptr对应一定是地址               *靠近哪一边都不错        
   // int  *       整型指针 类型           double *       char *

   - 指针变量指向谁，就把谁的地址赋值给指针变量
*/