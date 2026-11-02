#include <stdio.h>

int main() {
    int a, b;
    
    printf("请输入两个整数：");
    scanf("%d %d", &a, &b);
    
    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    
    if (b != 0) {
        printf("%d / %d = %.2f\n", a, b, (float)a / b);
        printf("%d %% %d = %d\n", a, b, a % b);
    } else {
        printf("除数不能为 0，无法进行除法和取余运算\n");
    }
    return 0;
}

