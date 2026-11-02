#include "main.h"
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);
    
    while (1)
    {
      printf("hello world\t\n");
        /* code */
    }
    
}