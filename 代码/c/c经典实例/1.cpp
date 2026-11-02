#include<stdio.h>
int main()
{
	int i,j,m;
	for(i=1;i<=4;i++)
	{
		for(j=1;j<=4;j++)
		{
			for(m=1;m<=4;m++)
			{
				if(i!=j&&(j!=m&&(m!=i)))
				{
					printf("%d%d%d\n",i,j,m);
				}
			}
		
		}	
	}
}

