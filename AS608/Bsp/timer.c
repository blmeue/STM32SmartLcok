#include "timer.h"

unsigned int sec;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        sec++;
        if(sec>=5)
        {
            sec=0;
           // SYN_FrameInfo(2,"[v7][m1][t5]Ω‚À¯ ß∞‹");
            
        }
    }
}

