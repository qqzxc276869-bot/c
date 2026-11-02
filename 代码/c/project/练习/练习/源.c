#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int data[50];
	int len;
}sqlist;

// 初始化：保留函数名init
void init(sqlist* list) {
	list->len = 0;
}

// 插入：保留函数名add
int add(sqlist* list, int index, int value)
{
	if (index < 1 || index > list->len + 1)
	{
		return 0;
	}
	if (list->len >= 50)
	{
		return 0;
	}
	for (int i = list->len; i >= index; i--)
	{
		list->data[i] = list->data[i - 1];
	}
	list->data[index - 1] = value;
	list->len++;
	return 1;
}

// 删除：保留函数名delete
int delete(sqlist* list, int index, int* value)
{
	if (index < 1 || index > list->len)
	{
		return 0;
	}
	// 先取出待删除元素，仅赋值一次
	*value = list->data[index - 1];
	for (int i = index; i < list->len; i++)
	{
		list->data[i - 1] = list->data[i];
	}
	list->len--;
	return 1;
}

int main()
{
	sqlist list;
	init(&list);
	return 0;
}
