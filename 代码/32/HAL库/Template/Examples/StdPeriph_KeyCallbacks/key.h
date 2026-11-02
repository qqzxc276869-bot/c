#ifndef KEY_H
#define KEY_H

#include "stm32f10x.h"
#include <stddef.h>

/* arg 是初始化时传入的用户参数。回调在主循环中执行。 */
typedef void (*KeyCallback)(void *arg);

typedef struct
{
    /* 配置：每个按键可以不同。 */
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t active_level;
    uint16_t debounce_ms;
    uint32_t long_ms;
    KeyCallback on_short;
    KeyCallback on_long;
    void *arg;

    /* 状态：每个对象独立保存，不要与其他按键共用。 */
    uint8_t raw_pressed;
    uint8_t stable_pressed;
    uint8_t long_sent;
    uint32_t raw_changed_ms;
    uint32_t pressed_ms;
} Key_t;

/* 先在应用层初始化输入 GPIO，再调用本函数绑定配置。 */
void Key_Init(Key_t *key,
              GPIO_TypeDef *port,
              uint16_t pin,
              uint8_t active_level,
              uint16_t debounce_ms,
              uint32_t long_ms,
              KeyCallback on_short,
              KeyCallback on_long,
              void *arg);

/* 主循环约每 5ms 调用一次；now_ms 是持续递增的毫秒时间。 */
void Key_Process(Key_t *key, uint32_t now_ms);

#endif
