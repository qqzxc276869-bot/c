#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define  MAX 100                // 空间个数
typedef  struct  sqlist {
	int  *data;
	int n;
	int len;  //顺序表长度            data中添加的元素的数
}sqlist;

//0.菜单
void help_menu() {
	printf("          欢迎操作线性表中---顺序表\n");
	printf("* ======================================================= *\n");
	printf("* 1. 初始化动态数组                                       *\n");
	printf("* 2. 释放动态数组                                         *\n");
	printf("* 3. 调整动态数组内存大小                                 *\n");
	printf("* 4. 获取动态数组元素个数                                 *\n");
	printf("* 5. 在指定位置指入新元素                                 *\n");
	printf("* 6. 删除指定位置的元素，并返回被删除的元素               *\n");
	printf("* 7. 遍历所有元素                                         *\n");
	printf("* 8. 在末尾添加元素                                        *\n");
	printf("* 9 清屏                                                  *\n");
	printf("* 10 退出                                                  *\n");
	printf("* ======================================================= *\n");
}

//1.创建顺序表     如果用指针定义的变量   动态开空间
//  传址  （直接定义二级指针  &S）              传值  (定义一级指针， 返回一个指针)
void initlist (sqlist* s, int n)
{
	s->data = (int*)(sqlist*)malloc(n*sizeof(int));
	if (s->data== NULL) {
		printf("空间开辟失败\n");
		return ;
	}
	s->n = n;
	s->len = 0;
	printf("动态顺序表初始化成功\n");
}

//2.释放动态数组    
void freelist(sqlist* s)
{
	free(s->data);
	s->data = NULL;
	s->len = 0;
	s->n = 0;
}
//3调整动态数组内存大小
int changelist(sqlist* s, int rewnum)
{
	if (s == NULL || rewnum <= 0)
		return -1;

	/* 防止缩小后丢失已有元素 */
	if (rewnum < s->len)
	{
		printf("新空间不能小于当前元素个数\n");
		return -1;
	}

	int* temp = realloc(s->data, rewnum * sizeof * temp);

	if (temp == NULL)
	{
		printf("调整空间失败\n");
		return -1;
	}

	s->data = temp;
	s->n = rewnum;

	return 0;
}
//4获取动态数组元素个数 
int getsize(sqlist *s)
{
	return s->len;
}
//5.在指定位置指入新元素
int addlist(sqlist* s, int index, int num)
{
	if (s == NULL || s->data == NULL)
		return -1;

	/* 插入允许index等于len，表示末尾插入 */
	if (index < 0 || index > s->len)
		return -1;

	if (s->len >= s->n)
	{
		int new_capacity = s->n == 0 ? 1 : s->n * 2;

		if (changelist(s, new_capacity) == -1)
			return -1;
	}

	/* 从后向前移动 */
	for (int i = s->len; i > index; i--)
	{
		s->data[i] = s->data[i - 1];
	}

	s->data[index] = num;
	s->len++;

	return 0;
}
//6.删除指定位置的元素，并返回被删除的元素 
int delectlist(sqlist* s, int index)
{
	if (index < 0 || index >= s->len)
	{
		return -1;
	}
	int delect = s->data[index];
	for (int i = index; i<s->len; i++)
	{
		s->data[i] = s->data[i+1];
	}
	s->len--;
	printf("删除的元素是%d\n",delect);	
	return 0;
}
//7.遍历所有元素  
void alllist(sqlist* s)
{
	for (int i =0;i<s->len; i++)
	{
		printf("%d\t", s->data[i]);
	}
	printf("\n");
}
//8.在末尾添加元素 
void addendlist(sqlist* s,int num)
{
	addlist(s, s->len, num);
}
int main() {
	sqlist s = { 0 };
	help_menu();
	int flag = 1;
	int newnum,index,cmd,size,num;
	while (flag) {
		printf("请输入一个菜单指令1~9：");
		scanf("%d",&cmd);
		switch (cmd) {
		case 1:
			//1.初始化动态数组
		initlist(&s,5);
			break;
		case 2:
			//2.释放动态数组
			freelist(&s);
			break;
		case 3:
			//3.调整动态数组内存大小 
			printf("请输入新的大小");
			scanf("%d", &newnum);
			changelist(&s,newnum);
			break;
		case 4:
			//4.获取动态数组元素个数 
			 size = getsize(&s);
			 printf("%d\n",size);
			break;
		case 5:
			//5.在指定位置指入新元素
			printf("输入要插入的位置");
			scanf("%d",&index);
			printf("输入要插入的元素");
			scanf("%d", &num);
			addlist(&s,index,num);
			break;
		case 6:
			//6.删除指定位置的元素，并返回被删除的元素 
			printf("删除元素的位置");
			scanf("%d",&index);
			delectlist(&s,index);

			break;
		case 7:
			//7.遍历所有元素    
			alllist(&s);
			break;
		case 8:
			//8.在末尾添加元素 
			 printf("添加的的元素");
			scanf("%d", &num);
			addendlist(&s,num);
			break;
		case 9:
			//6.清屏
			//system("pause");
			system("cls");
			help_menu();
			break;
		case 10:
			//7.退出
			flag = 0;
			break;
		default:
			//重新输入正确的指令
			break;
		}
	}
	return 0;
}
/*
一、线性表 -----逻辑结构来分它是线性结构

线性结构是一种数据结构，线性表典型的的线性结构
	  具有相同数据类型的n个数据元素的集合

	特点：
	1.存在唯一的第一个元素
	2.存在唯一的最后一个元素
	3.除了第一个元素外，其它元素，都有一个直接前驱
	4.除了最后一个元素外，其它元素，都有一个直接后继

	线性表的物理存储---顺序表      ，链表

	运算操作： 创键       增   删   改 查               销毁  等操作

二、顺序表
   1.各元素的逻辑顺序跟物理顺序一致，第i项就存在第i个位置。
		用数组方法来操作
  2.对顺序表中的所有元素，既可以顺序访问，也可以随机访问。
   for(int i=0;i<n;i++){
   }
  有索引号    arr[2]

   3.插入删除操作不方便，需要移动大量元素

  理解起来好理解，数组用的多

  三、操作顺序表
	  #define  MAX 100                 //空间个数
	 1. 定义结构体
	   typedef  struct  sqlist{
			int  data[MAX];
			int len;  //顺序表长度            data中添加的元素的数
	   }sql;

		typedef  struct  sqlist{
			int  * data;
			int n;//开辟几块空间  ，len==n   扩容
			int len;  //顺序表长度            data中添加的元素的数
	   }sql;


	   2. 定义结体变量 ，或者结构指针
	   sql  S;
	   sql * S=NULL;
*/
