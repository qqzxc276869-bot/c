
#pragma once
typedef  struct Student
{
	int id;
	char name[20];
	int age;
	char gender[10];
}student;
void Save(student stu[], int len);
int Load(student stu[]);
int add(student stu[], int* len);
int delet(student stu[], int* len);
void  Search(student stu[], int len);
void  Modify(student stu[], int len);
void  ShowStu(student stu[], int len);
int Show();
