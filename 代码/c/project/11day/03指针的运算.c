#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	//1. * 取值 （解引用）    & （取址）   
	  //&             在变量的基础上 向外走
	int  a = 10;
	printf("%d   %p\n", a, &a);//10   00000041287AFC24

	//*             在地址的基础上 向内走 *&a
	printf("%d\n", *&a);//10

	//问         &*a;  //错的            *&a //正确

	int* ptr = &a;
	printf("%p  %p\n", ptr, &a);//00000078F5EFF5B4  00000078F5EFF5B4

	printf("%p\n", &ptr);//00000061F71AF888   取址

	//取值

	printf("%d  %d", *ptr, *(*(&ptr)));//10  10

	//2.指针运算      指针变量+整数                整数（1） 步长值       1代表一个数据类型大小sizeof(数据类型)              //绝对
	printf("\n\n");
	int arr[5] = { 1,2,3,4,5 };
	int* ptr1 = arr;// &arr[0]
	printf("%p  %d\n", ptr1 + 1, *(ptr1 + 1));//0000004E6715FCDC  2
	printf("%p  %d\n", ptr1 + 4, *(ptr1 + 4));//0000004E6715FCE8  5
	  
	for (int i = 0; i < 5; i++) {
		printf("%p %d\n", ptr1 + i, *(ptr1 + i));
	}
	/*
	00000034122FFAD8 1
    00000034122FFADC 2
   00000034122FFAE0 3
   00000034122FFAE4 4
  00000034122FFAE8 5
	*/
	for (int i = 0; i < 5; i++) {
		printf("%p %d\n", arr + i, *(arr + i));
		//printf("%p %d\n", &arr[i], arr[i]);
	}


	//3. 指针  自增     自减       相对

	printf("\n\n");

	int* ptr2 = &arr[0];// arr;
	ptr2++;   //ptr2=ptr2+1
	++ptr2;   //ptr2=ptr2+1

	printf("%p  %d\n", ptr2, *ptr2);// 当前位置0000001D9E4FF840  3
	ptr2--;   //ptr2=ptr2-1
	--ptr2;
	printf("%p  %d\n", ptr2, *ptr2);// 当前位置0000001D9E4FF838  1
	printf("\n\n");
	ptr2 = &arr[0];   //重新归位
	for (int i = 0; i < 5; i++) {
		printf("%p  %d\n", ptr2, *ptr2);
		ptr2++;
	}
	return 0;
}

/*
指针的运算

    1.   掌握  * 取值 （解引用）    & （取址）
	     & * 优先级一样，  自右向左运算
		 *&a             &*p

	2.  指针变量+ 整数            指针变量- 整数

	3. 指针变量++       指针变量--
	  ++ 指针变量      --指针变量

*/
