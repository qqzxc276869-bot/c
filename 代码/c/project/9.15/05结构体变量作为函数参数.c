#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef  struct Student
{
	char name[20];
	int age;
	char gender[10];
}student;
int add(student stu[], int len)
{
	printf("请输入添加学生的 姓名 年龄 性别\n");
	
		scanf("%s", &stu[len].name);
		scanf("%d", &stu[len].age);
		scanf("%s", &stu[len].gender);
		printf("编号 姓名 年龄 性别\n");
		for (int i = 0; i < len+1; i++)
		{
			printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
		}
	return len+1;
}
int delet(student stu[], int len)
{
	
	char temp[20];
	printf("请输入删除学生的姓名\n");
	scanf("%s", &temp);
	for (int i = 0; i < len; i++)
	{
		/*int number = 0;*/
		
		if (strcmp(temp,stu[i].name)==0)
		{
			for (i; i < len - 1; i++)
			{
				stu[i] = stu[i + 1];
			}
			len--;
		}
	}
	printf("编号 姓名 年龄 性别\n");
	for (int i = 0; i < len + 1; i++)
	{
		printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
	}
	return len;
}
void  Search(student stu[], int len)
{
	char name[20];
	printf("请输入要查询学生的姓名");
	scanf("%s", name);
	for (int i = 0; i < len; i++)
	{
		if (strcmp(name, stu[i].name) == 0)
		{
			printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
		}
	}
}
 void  Modify(student stu[], int len)
{
	char name[20];
	printf("请输入要修改的学生姓名");
	scanf("%s", name);
	for (int i = 0; i < len; i++)
	{
		if (strcmp(name, stu[i].name) == 0)
		{
			printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
			printf("输入修改后的信息");
			scanf("%s    %d   %s", &stu[i].name, &stu[i].age, &stu[i].gender);
			printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
		}
	}
}
 void  ShowStu(student stu[],int len)
{
	printf("所有学生信息如下 \n");
	printf("编号 姓名 年龄 性别\n");
	for (int i = 0; i < len; i++)
	{
		printf("%d    %s    %d   %s\n", i + 1, stu[i].name, stu[i].age, stu[i].gender);
	}
}

int Show()
{
	printf("欢迎使用学生信息管理系统\n");
	printf("*======================*\n");
	printf("*1添加                 *\n");
	printf("*2显示                 *\n");
	printf("*3查询                 *\n");
	printf("*4修改                 *\n");
	printf("*5删除                 *\n");
	printf("*6清屏                 *\n");
	printf("*7退出                 *\n");
	printf("*======================*\n");

	return 0;
}
int main()
{
	student stu[100] = {
		{"a",18,"man"},{"b",19,"women"},{"c",20,"man"},{"d",20,"man"}
	}; 
	int inst=0,len=4,flag=1;
	Show();
	while (flag==1)
	{
		printf("请输入1~7的指令:");
		scanf("%d", &inst);
		switch(inst){
		case 1: len = add(stu, len);//添加
		break;
		case 2:  ShowStu(stu,len);//显示
			break;
		case 3:  Search(stu, len);//查询
			break;
		case 4:  Modify(stu, len);//修改
			break;
		case 5: len = delet(stu, len);//删除
			break;
		case 6:   system("cls");;//清屏
			break;
		case 7:  flag = 0;
			break;//退出
		}
	}

}       


