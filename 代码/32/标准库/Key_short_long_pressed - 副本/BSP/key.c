#include "key.h"
#include "stm32f10x_gpio.h"


uint8_t key_press(key *key,GPIO_TypeDef *gpiox,u16 GPIO_Pin,u16 press_state)
{
    u8 pin_state;
    u8 event;
    u8 press;
    pin_state = GPIO_ReadInputDataBit(gpiox, GPIO_Pin);
    if(pin_state==press_state)
    {
        press = 1;
    }
    else{
        press = 0;
    }
    switch (key->key_state)
    {
    case zero:
        if(press==1)
        {
            key->count = 1;
            key->key_state = one;
        };
        break;
    case one:
    if(press==1)
    {
        key->count++;
        if(key->count>=800)
        {
            key->key_state = two;
            return 2;
        }
    }
    else
    {
        if(key->count>=50)
        {
            event = 1;
        }
        else
        {
            event = 0;
        }
        key->count = 0;
        key->key_state = zero;
        return event;
    }
        break;

    case two:
        if (press==0)
        {
            key->count = 0;
            
            key->key_state = zero;
        }
        break;
    default :
         key->count = 0;   
        key->key_state = zero;
        break;
    }

    return 0;  /* 本次没有按键事件 */
}
