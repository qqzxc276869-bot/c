#include<stdio.h>
#include<stdint.h>
int main()
{

//将变量a的第2位设置为1，其他位保持不变
uint8_t a=0b10110011;
 printf("0x%02X\n",a|(1<<2));
 
 
// 将变量b的第2位、第6位设置为1，其他位保持不变
uint8_t b = 0b10110011; // 0xb3;
  printf("0x%02X\n",b|68);
  
  
 // 将变量c的第5位设置为0，其他位保持不变
uint8_t c = 0b10110011;  // 0xb3;
  printf("0x%02X\n",c&=~(1<<5));
  
  
// 将变量d的第0~3位设置为0，其他位保持不变
uint8_t d = 0b11111111;  // 0xff;
printf("0x%02X\n",d&=~15);


// 将变量e的第2位取反，其他位保持不变
uint8_t e = 0b10110011;  // 0xb3;
printf("0x%02X\n",e |(1<<2));


// 将变量f取出8-15位
uint32_t f = 0x12345678;
printf("0x%02X\n",(f>>8)&0xff); 
}


