#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "func.h"

void Save(student stu[], int len)
{
	FILE* fp = fopen("student.dat", "wb");

	if (fp == NULL)
	{
		printf("保存失败\n");
		return;
	}

	// 先保存当前学生人数
	fwrite(&len, sizeof(int), 1, fp);

	// 再保存学生数据
	fwrite(stu, sizeof(student), len, fp);

	fclose(fp);
}
int Load(student stu[])
{
	FILE* fp = fopen("student.dat", "rb");

	// 第一次运行，没有文件
	if (fp == NULL)
	{
		return -1;
	}
	int len = 0;
	// 读取人数
	fread(&len, sizeof(int), 1, fp);
	if (len < 0 || len > 100)
	{
		fclose(fp);
		return -1;
	}
	// 读取学生
	fread(stu, sizeof(student), len, fp);
	fclose(fp);
	return len;
}

int add(student stu[], int* len)
{
	printf("请输入添加学生的id\t姓名\t年龄\t性别\n");
	scanf("%d", &stu[*len].id);
	scanf("%s", stu[*len].name);
	scanf("%d", &stu[*len].age);
	scanf("%s", stu[*len].gender);
	printf("id\t姓名\t年龄\t性别\n");
	(*len)++;
	for (int i = 0; i < *len; i++)
	{
		printf("%d\t%s\t%d\t%s\n",stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
	}
	return *len;
}
int delet(student stu[], int* len)
{
	int id = 0, flag = 0;
	printf("请输入删除学生的id\n");
	for (int i = 0; i < len; i++)
	{
		printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
	}
	scanf("%d",&id);
	for (int i = 0; i <*len; i++)
	{
		/*int number = 0;*/

		if (id==stu[i].id)
		{
			for (i; i < *len - 1; i++)
			{
				stu[i] = stu[i + 1];
			}
			(*len)--;
		}
	}
	printf("id\t姓名\t年龄\t性别\n");
	for (int i = 0; i < *len; i++)
	{
		printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
	}
	return *len;
}
void  Search(student stu[], int len)
{
	int number = 0, flag = 1;
	printf("请输入要查询学生的id");
	while (flag) 
	{
		scanf("%d", &number);
		for (int i = 0; i < len; i++)
		{
			if (number == stu[i].id)
			{
				printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
				flag = 0;
				break;
			}
		}
		if (flag == 1)
		{
			printf("没找到此id学生,请重新输入\n");
		}
	}
}
void  Modify(student stu[], int len)
{
	int number = 0, flag = 1;;
	printf("请输入要修改的学生id\n");
	for (int i = 0; i < len; i++)
	{
		printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
	}
	printf("\n");
	while (flag) {
		scanf("%d", &number);
		for (int i = 0; i < len; i++)
		{
			if (number == stu[i].id)
			{
				printf("输入修改后的信息");
				scanf("%d\t%s\t%d\t%s", &stu[i].id, stu[i].name, &stu[i].age, stu[i].gender);
				printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
				flag = 0;
				break;
			}
			
		}
		if (flag == 1) 
		{
			printf("没找到此id学生,请重新输入\n");
		}
	}
}
void  ShowStu(student stu[], int len)
{
	printf("所有学生信息如下 \n");
	printf("id\t姓名\t年龄\t性别\n");
	for (int i = 0; i < len; i++)
	{
		printf("%d\t%s\t%d\t%s\n", stu[i].id, stu[i].name, stu[i].age, stu[i].gender);
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