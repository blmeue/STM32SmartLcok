#ifndef __MYUSART_H__
#define __MYUSART_H__

#include "main.h"
#include "usart.h"

extern uint8_t uart2_rx_buf[255];
extern unsigned char uart2_rx_len;
extern unsigned char receive_dat;

extern uint8_t uart1_rx_buf[255];
extern unsigned char uart1_rx_len;
extern unsigned char receive_dat1;
#endif
