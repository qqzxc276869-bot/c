#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int* fn() {
	int a = 10;
	a++;
	return &a;   //返回局部变量的地址或临时 : a
}

int* fn1() {
	static int a = 10;   //局部变量   空间开辟在全局    后续多次调用函数   a不能再赋值为10了，保持上一次内层中的数据
	a++;
	return &a;   //返回局部变量的地址或临时 : a
}

int* fn2() {
  int *ptr=	(int*)malloc(sizeof(int));
  if (ptr == NULL) {
	  printf("空间开辟失败\n");
	  return NULL;
   }
  *ptr = 20;
  return ptr;
}
int main() {
	//（1）. 定义指针，未初始化
	//int* ptr;
	//printf("%p", ptr, *ptr);

	//解决办法
	int* ptr=NULL;                  //地址是 0x0000000000000000             #define   NULL   0
	printf("%p\n ", ptr);

	//printf("%d ", *ptr);  // 不能操作这一块空间

	//(2)返回局部变量的地址
	//int* ptr1 = fn();
	//printf("%p\n", ptr1);
	//printf("%d\n", *ptr1);
	// 000000FF946FF734       -858993460


	 //解决办法 一   局部变量增加   static int a = 10;

	int* ptr1 = fn1();
	printf("%p\n", ptr1);
	printf("%d\n", *ptr1);

	ptr1 = fn1();
	printf("%p\n", ptr1);
	printf("%d\n", *ptr1);

	//解决办法   动态开辟空间        在堆区     
	int* ptr2 = fn2();
	printf("%p  %d\n", ptr2, *ptr2);//0000029B51C20850  20

	free(ptr2);
	ptr2 = NULL;


	//3.数组越界访问
	int arr[5] = { 1,2,3,4,5 };
	//for (int i = 0; i <= 5; i++) {
	//	printf("%d ", arr[i]);//1 2 3 4 5 -858993460
	//}
	//arr[5] = 100;

	//解决办法  不越界   i<5           arr[len-1]=100;

	return 0;
}

/*
野指针形成原因及避免办法

1.概念：
   野指针：就是指针指向的位置是不可知 （随机性   不正确  没有明确限制）
     随机性：  int * ptr;       地址是内存残留  不可知
	 不正确：指向已释放过空间   （局部变量    free()）
	 没有明确限制:  数组越界访问

 2.形成原因 （5个方面）
   （1） 定义指针，未初始化
    (2)  返回局部变量的地址

3.访问数组下标越界
  int arr[5];
  for(int i=0;i<=5;i++){
  
  }
  arr[5]=100;

  4.指针操作  ，注意有效性检查
   if(ptr!=NULL) {

   }

   5. 动态空间用了，不再使用  释放 了， 但是没有及进置NULL
   free(ptr);
   ptr=NULL;

 3.避免办法
	 1. 定义指针，及时初始化
	 2.解决办法 一   局部变量增加   static int a = 10;
	     办法二   动态开辟空间      返回地址
    3.访问数组不要下标越界
	int arr[5];
  for(int i=0;i<5;i++){

  }

  4.
  if(ptr==NULL) return;
  ....
  if(ptr!=NULL) {
   .....
   }

  5.free(ptr);
   ptr=NULL;
	 */