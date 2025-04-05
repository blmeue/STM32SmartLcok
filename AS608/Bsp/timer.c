#include "timer.h"

unsigned int sec;
unsigned int keep_sec=0;
unsigned char long_view=0;
unsigned int figuer_keep=0;

unsigned int key_user_sec=0;
unsigned int sec1=0;

unsigned int key_admin_sec=0;
unsigned int sec2=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
//        sec++;
//        if(sec>=10)
//        {
//            sec=0;
//            //SYN_FrameInfo(2,"[v7][m1][t5]解锁失败");  
//        }
        if(figure_flag==1)
        {
            figuer_keep++;
            if(figuer_keep==300)//30秒内不能使用指纹解锁
            {
                figuer_keep=0;
                figure_flag=0;
            }
        }
        
        if(key_user_flag==1)//密码输入错误三次，禁用密码输入
        {
            key_user_sec++;
            if(key_user_sec%10==0)
            {
                sec1++;
            }
            if(key_user_sec==300)//30秒内不能解锁
            {
                key_user_sec=0;
                sec1=0;
                key_user_flag=0;
                OLED_CLS();
                OLED_CLS();
                view=0;
            }
        }
        
        if(key_admin_flag==1)//管理员密码输入3次错误
        {
            key_admin_sec++;
            if(key_admin_sec%10==0)
            {
                sec2++;
            }
            if(key_admin_sec==300)//30秒内不能输入
            {
                key_admin_sec=0;
                sec2=0;
                OLED_CLS();
                key_admin_flag=0;
                OLED_CLS();
                
                view=18;
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

