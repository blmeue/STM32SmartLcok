#ifndef __HHSTORE_H
#define __HHSTORE_H
#include "stm32f1xx_hal.h"

# include "AS608.h"

//存储的起始地址
#define STORE_START_ADDRESS		0x0800F000

extern uint16_t Store_Data[];
extern uint16_t Store_Admin[];
extern uint16_t Store_Id[];

void Store_Init(void);
void Store_Save(void);
void Store_Clear(void);
uint32_t MyFLASH_ReadWord(uint32_t Address);
uint16_t MyFLASH_ReadHalfWord(uint32_t Address);


#endif
