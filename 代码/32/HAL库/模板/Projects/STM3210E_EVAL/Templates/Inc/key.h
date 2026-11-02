#ifndef KEY_H
#define KEY_H

#include <stdint.h>

void key_init(void);            /* 初始化 PE4 上拉输入。 */
uint8_t key_Getnum(void);       /* 按下并松开后返回 1。 */

#endif /* KEY_H */
