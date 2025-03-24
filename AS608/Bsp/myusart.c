#include "myusart.h"

uint8_t uart2_rx_buf[255];
unsigned char receive_dat;
unsigned char uart2_rx_len=0;

uint8_t uart1_rx_buf[255];
unsigned char receive_dat1;
unsigned char uart1_rx_len=0;

//��дUSART�жϷ�����򣬽�������ģ�鷢�͵�ָ���������õ�����ƺ�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // �������յ�������
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


