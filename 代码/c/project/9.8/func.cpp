#include <stdio.h>
int func() {

	for (int i = 101; i <= 200; i++)
	{
		int flag = 1;
		for (int j = 2; j < i; j++)
		{
			if (i % j == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
		{
			printf(" %d  \n", i);
		}
	}
	return 0;
}