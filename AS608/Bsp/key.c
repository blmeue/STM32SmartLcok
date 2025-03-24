#include "key.h"
#include "delay.h"
/*
*   以下代码可以实现按键的功能！！！！
*/

unsigned char KeySignal[4][4]={0};
int button4_4_Scan(void){
    unsigned char i,j;
    for(i=0;i<4;i++)
    {
        delay_ms(3);
        switch(i){
            case 0:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
                break;
            case 1:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
                break;
            case 2:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
                break;
            case 3:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
                break;
        }
        for(j=0;j<4;j++)
        {
            delay_ms(5);
            switch(j){
                case 0:
                    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)
                    {
                        KeySignal[i][j]=1;
                        while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0);
                    }
                    else
                    {
                        KeySignal[i][j]=0;
                    }
                    break;
                case 1:
                    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)
                    {
                        KeySignal[i][j]=1;
                        while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0);
                    }
                    else
                    {
                        KeySignal[i][j]=0;
                    }
                    break;
                case 2:
                    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)
                    {
                        KeySignal[i][j]=1;
                        while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0);
                    }
                    else
                    {
                        KeySignal[i][j]=0;
                    }
                    break;
                case 3:
                    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==0)
                    {
                        KeySignal[i][j]=1;
                        while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==0);
                    }
                    else
                    {
                        KeySignal[i][j]=0;
                    }
                    break;
            }
            
        }
       switch(i){
            case 0:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
                break;
            case 1:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
                break;
            case 2:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
                break;
            case 3:
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
                break;
        }
    }
        if(KeySignal[0][0]==1)return 13;
        else if(KeySignal[0][1]==1)return 14;
        else if(KeySignal[0][2]==1)return 15;
        else if(KeySignal[0][3]==1)return 16;
        else if(KeySignal[1][0]==1)return 9;
        else if(KeySignal[1][1]==1)return 10;
        else if(KeySignal[1][2]==1)return 11;
        else if(KeySignal[1][3]==1)return 12;
        else if(KeySignal[2][0]==1)return 5;
        else if(KeySignal[2][1]==1)return 6;
        else if(KeySignal[2][2]==1)return 7;
        else if(KeySignal[2][3]==1)return 8;
        else if(KeySignal[3][0]==1)return 1;
        else if(KeySignal[3][1]==1)return 2;
        else if(KeySignal[3][2]==1)return 3;
        else if(KeySignal[3][3]==1)return 4;
				
				else return 0;
}




