#include<stdio.h>
int main()
{
	int x,n,m,i,j;
//	x+100=n*n;
//	x+268=m*m;
//	m*m-n*n=168; 
//    i=m+n;
//    j=m-n;
//    i*j=168;
   for(i=2;i<85;i=i+2)
   {
   	for(j=2;j<85;j=j+2)
   	{
   		if(i>j&&i*j==168)
   		{
   			m=(i+j)/2;
   			n=(i-j)/2;
   			x=n*n-100;
   			printf("i=%d  j=%d x=%d\n",i,j,x);
		   }
	   }
   }
    
    
}


