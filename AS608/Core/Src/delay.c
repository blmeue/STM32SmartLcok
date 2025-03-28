#include "delay.h"
 
#define CPU_FREQUENCY_MHZ    72		// STM32时钟主频
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;
 
    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}
 
 
/*
 * 毫秒级延时
 * HAL函数HAL_Delay();
 */
void delay_ms(uint32_t ms)
{
    uint32_t i;
    for(i = 0; i < ms; i++)
        delay_us(1000);
}
