
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//void swap(int* a, int* b)
//{
//    int t= *a;
//    *a = *b;
//    *b = t;
//}
//int change(int a[], int low,int high)
//{
//    int mid = a[low];
//    int i = low, j = high;
//    while (i < j)
//    {
//        while (i<j && a[j]>=mid) j--;
//        while (i<j&& a[i] <=mid)i++;
//        swap(&a[i], &a[j]);
//    }
//    swap(&a[low], &a[i]);
//    return i;
//
//}
//void quick(int a[], int low, int high)
//{
//    if (low < high)
//    {
//        int pos = change(a, low, high);
//            quick(a, low, pos - 1);
//            quick(a, pos + 1, high);
//    }
//
//}
//
//
//int main() {
//    int a[9] = { 32, 2, 54, 6, 78, 23, 17, 76, 1 };
//    int n = sizeof(a) / sizeof(a[0]);
//    quick(a, 0, n - 1);
//    for (int i = 0; i < n; i++)
//    {
//        printf(" %d", a[i]);
//    }
//    return 0;
//}快排



 
//int  main()
//{
//	int a[9] = { 32, 2, 54, 6, 78, 23, 17, 76, 1 };
//	int len = 9;
//	for (int  i = 0; i <len-1 ; i++)
//	{
//		for (int j = 0; j < len - i - 1; j++)
//		{
//			if (a[j] > a[j+1])
//			{
//				int temp = a[j];
//				a[j] = a[j+1];
//				a[j+1] = temp;
//
//			}
//		}
//	}
//	for (int i = 0; i < len; i++)
//	{
//		printf("%d ", a[i]);
//	}
//}冒泡


//int  main()
//{
//	int a[9] = { 32, 2, 54, 6, 78, 23, 17, 76, 1 };
//	int len = 9;
//	for (int  i = 1; i <len ; i++)
//	{
//		int temp = a[i];
//		int j;
//		for (j = i - 1; j>=0&& a[j]>temp; j--)
//		{
//			a[j + 1] = a[j];
//		}
//		a[j + 1] = temp;
//
//	}
//	for (int i = 0; i < len; i++)
//	{
//		printf("%d ", a[i]);
//	}
//	}插入


int  main()
{
	int a[9] = { 32, 2, 54, 6, 78, 23, 17, 76, 1 };
	int len = 9, min, j, i;
	for (i = 0; i < len - 1; i++)
	{
		min = i;
		for (j = i + 1; j < len; j++)
		{
			if (a[j] < a[min])
			{
				min = j;
			}
		}
		if (min != i) 
		{
			int	temp = a[min];
			a[min] = a[i];
			a[i] = temp;


		}
		
	}
	for (int i = 0; i < len; i++)
	{
		printf("%d ", a[i]);
	}
}
//选择排序