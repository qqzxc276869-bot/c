#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define maxsize 10
typedef struct queue
{
	int data;
	struct queue* next;
}queue;
typedef struct {
	queue* front;
	queue* end;
	int size;
}qunode;
//初始化
void init(qunode* q)
{
	if (q == NULL)
	{
		return;
	}
	q->front = NULL;
	q->end = NULL;
	q->size = 0;
}
int empty(qunode* q)
{
	if (q == NULL)
	{
		return 1;
	}
	return q->front == NULL;
}
int push(qunode* q,int value)
{
	queue* temp;
	if (q == NULL || value == NULL)
	{
		return 0;
	}
	temp = malloc(sizeof(*temp));
	if (temp == NULL)
	{
		printf("空间开辟失败");
		return 0;
	}
	temp->data = value;
	temp->next = NULL;
	if (q->end == NULL)
	{
		q->end = temp;
		q->front = temp;
	}
	else
	{
		q->end->next = temp;
		q->end = temp;
	}
	q->size++;
	printf("入队成功\n");
}
int out(qunode* q, int* value)
{
	queue* temp;
	if (q == NULL || value == NULL)
	{
		return 0;
	}
	if (empty(q))
	{
		printf("队列为空\n");
		return 0;
}
	temp = q->front;
	*value = temp->data;
	q->front = temp->next;
	if (q->front == NULL)
	{
		q->end = NULL;
	}
	free(temp);
	q->size--;
	return 1;
}
int gethead(qunode* q, int* value)
{
	queue* temp;
	if (q == NULL)
	{
		return 0;
	}
	if (empty(q))
	{
		printf("队列为空\n");
		return 0;
	}
	value=q->front->data;
	return 1;
}
int getend(qunode* q, int* value)
{
	queue* temp;
	if (q == NULL)
	{
		return 0;
	}
	if (empty(q))
	{
		printf("队列为空\n");
		return 0;
	}
	value = q->end->data;
	return 1;
}
void print(qunode *q)
{
	queue *temp;
	if (q == NULL)
	{
		return;
	}if (empty(q))
	{
		printf("队列为空");
		return;
	}
	temp = q->front;
	while (temp != NULL)
	{
		printf("%d\t", temp->data);
		temp = temp->next;
	}
	printf("\n");
}
int size(qunode* q)
{
	if (q == NULL)
	{
		return 0;
	}
	return q->size;
}
void destroy(qunode* q)
{
	queue* temp;
	while (q->front != NULL)
	{
		temp = q->front;
		q->front = temp->next;
		free(temp);
	}
	q->front = NULL;
	q->end = NULL;
	q->size = 0;
	printf("销毁成功\n");
}

void menu()
{
	printf("\n========= 队操作 =========\n");
	printf("1. 入队\n");
	printf("2. 出队\n");
	printf("3. 获取元素个数\n");
	printf("4. 获取队头元素\n");
	printf("5. 获取队尾元素\n");
	printf("6. 获取队列\n");
	printf("7. 销毁队列\n");
	printf("8. 清屏\n");

	printf("0. 退出\n");
	printf("==========================\n");

}
int main()
{
	qunode q;
	init(&q);
	int value,order;
	menu();
	while (1)
	{
		printf("请输入指令 ");
		scanf("%d", &order);
		switch (order)
		{
		case 0://退出
			return 1;

		case 1://入队
			printf("请输入元素:");
			scanf("%d", &value);
			push(&q, value);
		break;
		case 2://出队
			out(&q, &value);
			printf("出队元素是%d\n", value);
		break;
		case 3://获取元素个数
			printf("队列有%d个元素\n", size(&q));
		break;
		
		case 4://获取队头元素
			printf("队头元素为%d \n", value);
		break;
		case 5://获取队尾元素
			printf("队尾元素为%d \n",value);
			break;
		case 6://打印队列
			print(&q);
			break;
		case 7://销毁队列
			destroy(&q);
			break;
		case 8:
			system("cls");
			menu();
		break;
		}
	}
}

