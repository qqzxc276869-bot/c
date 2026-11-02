#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
typedef struct
{
	int data[5];
	int top;
}Stack;
void init(Stack* stack)//初始化
{
	if (stack == NULL)
	{
		return;
	}
	stack->top = -1;

}
int empty(Stack* stack)
{
	
	return stack->top == -1;
}
int full(Stack* stack)
{
	
	return stack->top ==4;
}
int push(Stack* stack,int value)
{
	if (stack == NULL)
	{
		return 0;
	}
	if (full(stack))
	{
		printf("栈已满\n");
		return 0;
	}
	 stack->top++;
	stack->data[stack->top] = value;
	printf("入栈成功\n");
	return 1;
}
int out(Stack* stack,int *value)
{
	if (stack == NULL || value == NULL)
	{
		return 0;
	}
	if (empty(stack))
	{
		printf("栈为空，无法出栈\n");
		return 0;
	}
	*value = stack->data[stack->top];
	stack->top--;
	return 1;
}
int get(Stack* stack,int *value)
{
	if (stack == NULL || value == NULL)
	{
		return 0;
	}
	if (empty(stack))
	{
		printf("栈为空，无法出栈\n");
		return 0;
	}
	*value = stack->data[stack->top];
	return 1;
}
int print(Stack* stack)
{


	if (stack == NULL)
	{
		return;
	}

	if (empty(stack))
	{
		printf("栈为空\n");
		return;
	}

	printf("栈顶\n");
	printf(" ↓\n");
	for (int i = stack->top; i >= 0; i--)
	{
		printf("[%d]\n", stack->data[i]);
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
	Stack stack;
	init(&stack);
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
			push(&stack, value);
			break;
		case 2://出栈
		
			out(&stack, &value);
			printf("出栈的元素: %d\n",value);
			break;
		case 3://获取栈顶元素
			get(&stack, &value);
			printf("栈顶元素: %d\n", value);
			break;
		case 4://打印
			print(&stack);
			break;
		case 5:
			system("cls");
			menu();
			break;
		case 0:
			return;

		}
	}
}

