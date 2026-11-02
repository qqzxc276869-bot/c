#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct Stack
{
	int data;
	struct Stack* next;
}Stack;
void init(Stack** top)//初始化
{
	if (top == NULL)
	{
		return;
	}
	*top = NULL;

}
int empty(Stack*top)
{
	
	return top == NULL;
}

int push(Stack**top,int value)
{
	Stack* ptr;
	if (top == NULL)
	{
		return 0;
	}
	ptr = malloc(sizeof(Stack));
	if (ptr == NULL)
	{
		printf("申请失败");
		return 0;
	}
	ptr->data = value;
	ptr->next = *top;
	*top = ptr;
	printf("入栈成功\n");
	return 1;
}
int out(Stack** top,int *value)
{
	if (top == NULL || value == NULL)
	{
		return 0;
	}
	if (*top == NULL)
	{
		printf("栈为空，无法出栈\n");
		return 0;
	}
	Stack* ptr;
	ptr = *top;
	*value = (*top)->data;
	*top = ptr->next;
	free(ptr);
	return 1;
}
int get(Stack*top,int *value)
{
	if (top == NULL || value == NULL)
	{
		return 0;
	}
	if (empty(top))
	{
		printf("栈为空，无法出栈\n");
		return 0;
	}
	*value = top->data;
	return 1;
}
void print(Stack* top)
{

	Stack* ptr;
	if (top == NULL)
	{
		return ;
	}

	if (empty(top))
	{
		printf("栈为空\n");
		return ;
	}
	ptr = top;
	printf("栈顶\n");
	printf(" ↓\n");
	while(ptr != NULL)
	{
		printf("%d\t", (void*)ptr->data);
		ptr = ptr->next;
	}
	printf("栈底\n");

}
void menu()
{
	printf("\n========= 栈操作 =========\n");
	printf("1. 入栈\n");
	printf("2. 出栈\n");
	printf("3. 获取栈顶元素\n");
	printf("4. 打印栈\n");
	printf("5. 清屏\n");
	printf("0. 退出\n");
	printf("==========================\n");

}
int main()
{
	Stack *top=NULL;												
	init(&top);
	int order,value;
	menu();
	while (1)
	{
		printf("请输入指令：");
		scanf("%d", &order);
		switch (order)
		{
		case 1://入栈
			printf("请输入入栈的元素 :");
			scanf("%d", &value);
			push(&top, value);
			break;
		case 2://出栈
			if (out(&top, &value))
			{
				printf("出栈的元素：%d\n", value);
			}
			break;
		case 3://获取栈顶元素
			get(top, &value);
			printf("栈顶元素: %d\n", value);
			break;
		case 4://打印
			print(top);
			break;
		case 5:
			system("cls");
			menu();
			break;
		case 0:
			return 0;

		}
	}
}

