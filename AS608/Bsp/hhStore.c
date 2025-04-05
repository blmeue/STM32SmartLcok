#include "hhStore.h"

//存储的起始地址
//#define STORE_START_ADDRESS		0x0800FC00
//存储数据的个数
#define STORE_COUNT				17
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

  //FLASH读取一个16位的字
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

  //FLASH读取一个8位的字
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));	//使用指针访问指定地址下的数据并返回
}
void hhMyFLASH_ErasePage(uint32_t ErasePageBaseAddr,uint32_t ErasePageNbPageCount)
{
       HAL_FLASH_Unlock();
       FLASH_EraseInitTypeDef EraseInitStruct = {
           .TypeErase = FLASH_TYPEERASE_PAGES,       //页擦除
           .PageAddress = ErasePageBaseAddr,                //擦除地址
           .NbPages = ErasePageNbPageCount                             //擦除页数
       };
       uint32_t PageError = 0;
       __disable_irq();                             //擦除前关闭中断
       if (HAL_FLASHEx_Erase(&EraseInitStruct,&PageError) == HAL_OK)
       {
//           printf("擦除 成功\r\n");
       }
       __enable_irq();
	   HAL_FLASH_Lock();					//加锁
}
//定义SRAM数组
uint16_t Store_Data[6]={1,2,3,4,5,6};	
uint16_t Store_Admin[6]={8,8,8,8,8,8};
uint16_t Store_Id[3]={0,0,0};

//初始化假如没初始化过就初始化(第一个半字写特殊标志位0xA5A5，其余写入0)，假如初始化过就把闪存数据读取到数组Store_Data中
void Store_Init(void)
{
	/*判断是不是第一次使用*/
	if (MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)	//读取第一个半字的标志位，if成立，则执行第一次使用的初始化
	{
		hhMyFLASH_ErasePage(STORE_START_ADDRESS,1);					//擦除指定页
		HAL_FLASH_Unlock();
		//MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);	//在第一个半字写入自己规定的标志位，用于判断是不是第一次使用
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS,0xA5A5);
		for (uint32_t i = 1; i < STORE_COUNT; i ++)				//循环STORE_COUNT次，除了第一个标志位
		{
//			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);		//除了标志位的有效数据全部清0
            if(i<7){
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,i);
            }
            else if(i>=7&&i<13)
            {
               HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Admin[i-7]); 
            }
            else if(i==13)
            {
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,1);
            }
            else if(i>13&&i<19)
            {
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,0);
            }
		}
		HAL_FLASH_Lock();
	}
	
	/*上电时，将闪存数据加载回SRAM数组，实现SRAM数组的掉电不丢失*/
    else {
	for (uint16_t i = 1; i < STORE_COUNT; i ++)					//循环STORE_COUNT次，包括第一个标志位
	{
        if(i<7){
		    Store_Data[i-1] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);		//将闪存的数据加载回SRAM数组
        }
        else if(i>=7&&i<13)
        {
            Store_Admin[i-7] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);		//将闪存的数据加载回SRAM数组
        }
        else if(i==13)
        {
            ID_NUM_store = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
        }
        else if(i>=14)
        {
            Store_Id[i-14]=MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
        }
	}
  }
}
//数组保存到闪存
void Store_Save(void)
{
	hhMyFLASH_ErasePage(STORE_START_ADDRESS,1);				//擦除指定页
	HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS,0xA5A5);
    //HAL_FLASH_Program（FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS,0xA5A5);
	for (uint16_t i = 1; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，包括第一个标志位
	{
//		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);	//将SRAM数组的数据备份保存到闪存
        if(i<7){
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Data[i-1]);
        }
        else if(i>=7 &&i<13)
        {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Admin[i-7]);
        }
        else if(i==13)
         {
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,ID_NUM_store);
         }
         else if(i>=14&&i<19)
         {
             HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Id[i-14]);
         }
	}
	HAL_FLASH_Lock();
}

void Store_Clear(void)
{
	for (uint32_t i = 1; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，除了第一个标志位
	{
        if(i<7){
            Store_Data[i-1] = i;							//SRAM数组有效数据重置为123456
            //HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,i);
        }
        else if(i>=7&&i<13)
        {
            Store_Admin[i-7]=8;
           // HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Admin[i-7]); 
        }
        else if(i==13)
        {
            ID_NUM_store=0;
           // HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,1);
        }
        else if(i>13&&i<17)
        {
                Store_Id[i-14]=0;
        }
	}
	Store_Save();										//保存数据到闪存
}
