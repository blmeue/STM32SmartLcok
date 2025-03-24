#include "myusart.h"

uint8_t uart2_rx_buf[255];
unsigned char receive_dat;
unsigned char uart2_rx_len=0;

uint8_t uart1_rx_buf[255];
unsigned char receive_dat1;
unsigned char uart1_rx_len=0;

//编写USART中断服务程序，接收蓝牙模块发送的指令，解析后调用电机控制函数。
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // 解析接收到的数据
    if(huart->Instance == USART2)
    {
        uart2_rx_buf[uart2_rx_len++] = receive_dat;
        HAL_UART_Receive_IT(&huart2,&receive_dat,1);
    }
    if(huart->Instance == USART1)
    {
        uart1_rx_buf[uart1_rx_len++] = receive_dat1;
        HAL_UART_Receive_IT(&huart1,&receive_dat1,1);
    }
}


