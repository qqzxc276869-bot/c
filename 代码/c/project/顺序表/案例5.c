#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define  MAX 100                // 空间个数
typedef  struct  sqlist {
	int  data[MAX];
	int len;  //顺序表长度            data中添加的元素的数
}sqlist;

//0.菜单
void help_menu() {
	printf("\n");
	printf("\t欢迎操作顺序表\n");
	printf("*=======================================*\n");
	printf("* 1. 创建顺序表\t\t\t\t*\n");
	printf("* 2. 打印顺序表\t\t\t\t*\n");
	printf("* 3. 追加一个节点\t\t\t\t*\n");
	printf("* 4. 插入一个节点\t\t\t\t*\n");
	printf("* 5. 删除一个节点\t\t\t\t*\n");
	printf("* 6. 修改一个节点\t\t\t\t*\n");
	printf("* 7. 查找顺序表中的元素\t\t\t\t*\n");
	printf("* 8. 销毁顺序表\t\t\t\t*\n");
	printf("* 9 清屏\t\t\t\t*\n");
	printf("* 10. 退出\t\t\t\t*\n");
	printf("*=======================================*\n");
	printf("\n");
}

//1.创建顺序表     如果用指针定义的变量   动态开空间
//  传址  （直接定义二级指针  &S）              传值  (定义一级指针， 返回一个指针)
sqlist* createList() {
	sqlist* s = (sqlist*)malloc(sizeof(sqlist));
	if (s == NULL) {
		printf("空间开辟失败\n");
		return NULL;
	}
	//开辟成功   结构初始一下
	s->len = 0;//顺序的长度
	printf("顺序表初始化成功\n");
	return s;

}

//2.打印顺序表   assert()         assert.h
void printList(sqlist* s) {
	if (s== NULL) {
		printf("空间开辟失败\n");
		return;
	}
	for (int i = 0; i < s->len; i++) {
		printf("%d\t", s->data[i]);
	}
	printf("\n");
}
void addlist(sqlist* s,int num)
{
	if (s->len >= MAX)
	{
		printf("空间已满\n");
		return;
	}
	s->data[s->len] = num;
	s->len++;
	printf("追加成功\n");
}
void insertlist(sqlist* s, int num, int index)
{
	if (index < 0 || index >= s->len)
	{
		printf("超出范围\n");
		return;
	}
	if (s->len >= MAX)
	{
		printf("空间已满\n");
		return;
	}
	for (int i = s->len; i >= index; i--)
	{
		s->data[i + 1] = s->data[i];
	}
	s->data[index] = num;
	printf("插入成功\n");
	s->len++;
}
void delectlist(sqlist* s, int index)
{
	if (index < 0 || index >= s->len)
	{
		printf("超出范围\n");
		return;
	}
	for (int i = s->len; i >= index; i--)
	{
		s->data[i] = s->data[i+1];
	}
	s->len--;
	printf("删除成功\n");
}
void changelist(sqlist* s, int num, int index)
{
	if (index < 0 || index >= s->len)
	{
		printf("超出范围\n");
		return;
	}
	s->data[index] = num;
	printf("修改成功\n");
}
void findlist(sqlist* s, int num, int index)
{
	if (index < 0 || index >= s->len)
	{
		printf("超出范围\n");
		return;
	}
	printf("%d对应的的值为%d\n", index, s->data[index]);
}
int main() {
	sqlist* s = NULL;
	help_menu();
	int flag = 1;
	int num,index;
	int cmd;
	while (flag) {
		printf("请输入一个菜单指令1~7：");
		scanf("%d", &cmd);
		switch (cmd) {
		case 1:
			//1.创建顺序表
			s= createList();
			break;
		case 2:
			//2.打印顺序表
			printList(s);
			break;
		case 3:
			//3.追加一个节点
			printf("请输入追加的值");
			scanf("%d",&num);
			addlist(s, num);
			break;
		case 4:
			//4.插入一个节点
			printf("插入的值");
			scanf("%d",&num);
			printf("插入的位置0~%d",s->len);
			scanf("%d",&index);
			insertlist(s, num,index);
			break;

			break;
		case 5:
			//5.删除一个节点
			printf("删除的位置");
			scanf("%d", &index);
			delectlist(s,index);
			break;
		case 6:
			//6.修改一个节点
			printf("修改的值");
			scanf("%d", &num);
			printf("修改元素的位置");
			scanf("%d", &index);
			changelist(s, num, index);
			break;
		case 7:
			//7.查找顺序表中的元素
			printf("查找元素的值");
			scanf("%d", &num);
			printf("查找元素的位置");
			scanf("%d", &index);
			findlist(s, num, index);
			break;
		case 8:
			//8.销毁顺序表
			free(s);
			s = NULL;
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
