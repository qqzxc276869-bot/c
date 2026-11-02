#include <reg52.h>
#include <string.h>

/*
 * 串口命令控制示例：
 *   发送 "ON\r\n"  ：翻转 P2 各引脚的输出状态，并回复 "ok, on"。
 *   发送 "OFF\r\n" ：将 P2 置为高电平，并回复 "ok, off"。
 *   其他命令       ：回复 "err"。
 *
 * 一条命令以 '\r' 或 '\n' 结束，接收到的字符由串口中断存入 rec_buf，
 * 主循环再使用 strcmp() 比较完整命令。
 */

/* 为便于阅读定义的无符号整数类型。 */
typedef unsigned char u8;     /* 8 位：0 ~ 255 */
typedef unsigned int  u16;    /* 16 位：0 ~ 65535 */

/*
 * 简单的忙等待延时。
 * 延时时间会受晶振频率和编译器优化影响，不能认为 ten_us 精确等于 10 us。
 */
void delay_us(u16 ten_us)
{
    while (ten_us--);
}

/*
 * 简单的忙等待毫秒延时。
 * 本函数未在当前程序中使用；循环次数需按实际晶振和编译结果校准。
 */
void delay_ms(u16 ms)
{
    u16 i, j;

    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--);
}

/*
 * 使用定时器 1 初始化串口 1。
 * baud 参数并非“波特率数值”，而是定时器 1 模式 2 的重装载值。
 *
 * 当前 main() 传入 0xFA，并设置 SMOD=1：
 *   - 晶振为 11.0592 MHz 时，波特率为 9600 bps；
 *   - 晶振为 12.000 MHz 时，实际约为 10416.7 bps，并非 9600 bps。
 */
void uart_init(u8 baud)
{
    /* 串口方式 1：8 位数据、1 位起始位、1 位停止位；REN=1 允许接收。 */
    SCON = 0x50;

    /* 设置 SMOD=1，使定时器 1 产生的串口波特率加倍。
       直接赋值会同时清除 PCON 中的其他控制位。 */
    PCON = 0x80;

    /* 定时器 1 工作在模式 2（8 位自动重装载）。
       先保留定时器 0 的配置，再清除并设置定时器 1 的高四位。 */
    TMOD &= 0x0F;
    TMOD |= 0x20;

    /* 首次计数值与自动重装载值均由 baud 指定。 */
    TL1 = baud;
    TH1 = baud;

    /* 只使用定时器 1 的溢出作为波特率时钟，不使用定时器 1 中断。 */
    ET1 = 0;

    /* 允许串口中断和总中断，并启动定时器 1。 */
    ES = 1;
    EA = 1;
    TR1 = 1;
}

/*
 * 阻塞方式发送一个字节。
 * 写入 SBUF 后，硬件发送完整个串口帧会将 TI 置 1；随后清除 TI，
 * 以便下一次发送继续等待新的发送完成事件。
 */
void send_char(u8 byte)
{
    SBUF = byte;
    while (!TI);
    TI = 0;
}

/* 依次发送以 '\0' 结尾的字符串，不发送结尾的 '\0' 本身。 */
void send_string(u8 *str)
{
    while (*str != '\0')
    {
        send_char(*str);
        str++;
    }
}

/* 一条串口命令最多保存 BUF_LEN - 1 个有效字符，最后一个字节留给 '\0'。 */
#define BUF_LEN 20

/*
 * rec_buf：由串口中断写入、由主循环读取的命令缓冲区。
 * rec_len：当前已保存的字符数量。
 * is_complate：一条完整命令到达标志；名称保留原代码拼写，含义为 complete。
 */
char rec_buf[BUF_LEN];
u8 rec_len = 0;
u8 is_complate = 0;

void main()
{
    /* 0xFA + SMOD=1 仅在 11.0592 MHz 晶振下对应 9600 bps。 */
    uart_init(0xFA);

    /* 常见开发板 LED 为低电平点亮，0xFF 表示全部熄灭。 */
    P2 = 0xFF;

    while (1)
    {
        /* 串口中断接收到一条以回车或换行结束的命令后置位该标志。 */
        if (is_complate)
        {
            is_complate = 0;

            /* strcmp() 返回 0 表示两个字符串内容相同。 */
            if (strcmp(rec_buf, "ON") == 0)
            {
                /* 按位取反：P2 每一位的状态都会翻转，不是“全部点亮”。 */
                P2 = ~P2;
                send_string("ok, on\r\n");
            }
            else if (strcmp(rec_buf, "OFF") == 0)
            {
                /* 对低电平点亮的 LED，P2=0xFF 表示全部熄灭。 */
                P2 = 0xFF;
                send_string("ok, off\r\n");
            }
            else
            {
                /* 命令不是 ON 或 OFF。 */
                send_string("err\r\n");
            }
        }
    }
}

/* 串口 1 中断服务函数：中断号 4。 */
void uart() interrupt 4
{
    u8 rec = 0;

    /* RI=1 表示接收寄存器中已有一个新字节。 */
    if (RI)
    {
        rec = SBUF;

        /* '\r' 或 '\n' 视为命令结束符。
           对 "ON\r\n"，处理 '\r' 后 rec_len 清零，紧随其后的 '\n' 会被忽略。 */
        if (rec == '\r' || rec == '\n')
        {
            /* 空行不产生命令；非空命令末尾补 '\0' 以便 strcmp() 使用。 */
            if (rec_len > 0)
            {
                rec_buf[rec_len] = '\0';
                is_complate = 1;
                rec_len = 0;
            }
        }
        else
        {
            /* 预留一个字节给字符串结束符，避免数组越界。 */
            if (rec_len < BUF_LEN - 1)
            {
                rec_buf[rec_len] = rec;
                rec_len++;
            }
            else
            {
                /* 缓冲区已满：丢弃当前这条未完成命令，等待重新输入。 */
                memset(rec_buf, 0, rec_len);
                rec_len = 0;
            }
        }

        /* 清除接收完成标志，允许硬件报告下一个接收字节。 */
        RI = 0;
    }
}
