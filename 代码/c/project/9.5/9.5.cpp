#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	/*输入一行字符，分别统计出其中英文字母、空格、数字和其它字符的个数。
		程序分析：利用while语句, 条件为输入的字符不为'\n'。*/
	char c;
	int letter = 0, space = 0, digit = 0, other = 0;
	printf("请输入一行字符：");
	
	while((c = getchar())!= '\n')
	{
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
			letter++;
		else if (c == ' ')
			space++;
		else if (c >= '0' && c <= '9')
			digit++;
		else
			other++;		
	}	
	printf("英文字母个数：%d\n", letter);
	printf("空格个数：%d\n", space);
	printf("数字个数：%d\n", digit);
	printf("其他符号 %d", other);
}