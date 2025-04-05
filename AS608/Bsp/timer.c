#include "timer.h"

unsigned int sec;
unsigned int keep_sec=0;
unsigned char long_view=0;
unsigned int figuer_keep=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        sec++;
        if(sec>=5)
        {
            sec=0;
           // SYN_FrameInfo(2,"[v7][m1][t5]解锁失败");  
        }
        if(figure_flag==1)
        {
            figuer_keep++;
            if(figuer_keep==300)//30秒内不能使用指纹解锁
            {
                figuer_keep=0;
                figure_flag=0;
            }
        }
        if(keep_menu==1)
        {
            keep_sec++;
            if(keep_sec==600)//1分钟无操作，自动上锁,因为按键扫描那里用掉了一部分时间，所以这里不需要计时1000
            {
                long_view=1;
                keep_sec=0;
            }
        }
        else
        {
            keep_sec=0;
        }
    }
}

