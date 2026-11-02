#include "key.h"
#include "stm32f10x_gpio.h"

void Key_Init(Key_t *key,
              GPIO_TypeDef *port,
              uint16_t pin,
              uint8_t active_level,
              uint16_t debounce_ms,
              uint32_t long_ms,
              KeyCallback on_short,
              KeyCallback on_long,
              void *arg)
{
    key->port = port;
    key->pin = pin;
    key->active_level = active_level;
    key->debounce_ms = debounce_ms;
    key->long_ms = long_ms;
    key->on_short = on_short;
    key->on_long = on_long;
    key->arg = arg;

    key->raw_pressed = 0;
    key->stable_pressed = 0;
    key->long_sent = 0;
    key->raw_changed_ms = 0;
    key->pressed_ms = 0;
}

void Key_Process(Key_t *key, uint32_t now_ms)
{
    uint8_t pressed;
    uint32_t held_ms;

    /* 把不同的有效电平统一为：1 按下，0 松开。 */
    pressed = (GPIO_ReadInputDataBit(key->port, key->pin)
               == key->active_level) ? 1U : 0U;

    /* 采样状态改变后，从这一刻重新计算消抖时间。 */
    if (pressed != key->raw_pressed)
    {
        key->raw_pressed = pressed;
        key->raw_changed_ms = now_ms;
    }

    /* 只有新状态保持足够久，才更新稳定状态。 */
    if ((key->raw_pressed != key->stable_pressed) &&
        ((uint32_t)(now_ms - key->raw_changed_ms) >= key->debounce_ms))
    {
        key->stable_pressed = key->raw_pressed;

        if (key->stable_pressed != 0U)
        {
            /* 确认按下后，使用本次稳定按下的起始采样时间。 */
            key->pressed_ms = key->raw_changed_ms;
            key->long_sent = 0;
        }
        else
        {
            /* 使用松开开始时间，避免把松开消抖也计入按住时长。 */
            held_ms = (uint32_t)(key->raw_changed_ms - key->pressed_ms);

            if (key->long_sent == 0U)
            {
                if (held_ms >= key->long_ms)
                {
                    /* 防止扫描恰好错过长按阈值。 */
                    key->long_sent = 1;
                    if (key->on_long != NULL)
                    {
                        key->on_long(key->arg);
                    }
                }
                else if (key->on_short != NULL)
                {
                    key->on_short(key->arg);
                }
            }
        }
    }

    /* 持续按住：达到阈值触发一次；开始松开消抖时暂停触发。 */
    if ((key->stable_pressed != 0U) &&
        (key->raw_pressed != 0U) &&
        (key->long_sent == 0U) &&
        ((uint32_t)(now_ms - key->pressed_ms) >= key->long_ms))
    {
        key->long_sent = 1;
        if (key->on_long != NULL)
        {
            key->on_long(key->arg);
        }
    }
}
