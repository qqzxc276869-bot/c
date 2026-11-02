#include<stdio.h>
int main()
{
	double  reward,reward1,reward2,reward3,reward4,reward5,reward6;
	int money;
	printf("请输入当月利润:" );
	scanf("%d",&money);
	reward1=100000*0.1;
	reward2=reward1+100000*0.075;
	reward3=reward2+200000*0.05;
	reward4=reward3+200000*0.03;
	reward5=reward4+400000*0.015;
	if(money<=100000)
	{
		reward=0.1*money;
	 }
	else if(money<=200000)
	{
		reward=reward1+(money-100000)*0.075;
	 }
	  else if(money<=400000)
	{
		reward=reward2+0.05*(money-200000);
	 }
	  else if(money<=600000)
	{
		reward=reward3+0.03*(money-400000);
	 }
	   else if(money<=1000000)
	{
		reward=reward4+0.0015*(money-600000);
	 }
	    else
	{
		reward=reward5+0.01*(money-1000000);
	 }
	 printf("应发奖金为：%f" ,reward);     

}

