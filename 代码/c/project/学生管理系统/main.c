#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "func.h"

int main()
{
	student stu[100] = {
		{1,"a",18,"man"},{2,"b",19,"women"},{3,"c",20,"man"},{4,"d",20,"man"}
	};
	int inst = 0, len = 4, flag = 1;
	int temp = Load(stu);

	if (temp != -1)
	{
		len = temp;
	}
	Show();
	while (flag == 1)
	{
		printf("请输入1~7的指令:");
		scanf("%d", &inst);
		switch (inst) {
		case 1: add(stu, &len);//添加
			Save(stu, len);
			break;
		case 2:  ShowStu(stu, len);//显示
			break;
		case 3:  Search(stu, len);//查询
			break;
		case 4:  Modify(stu, len);//修改
			Save(stu, len);
			break;
		case 5: delet(stu, &len);//删除
			Save(stu, len);
			break;
		case 6:   system("cls");
			Show();//清屏
			break;
		case 7:  flag = 0;
			Save(stu, len);
			break;//退出
		}
	}

}
