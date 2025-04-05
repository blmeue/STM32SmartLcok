/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "AS608.h"
#include "oled.h"
#include "key.h"
#include "string.h"
#include "hhStore.h"
#include "menu.h"
#include "myusart.h"
#include "string.h"
#include "SYN6288.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

void key_show(void);
int keynum0;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

int fputc(int c, FILE* stream)
{
	uint8_t ch[]={c};
	HAL_UART_Transmit(&huart1,ch,1,0xffff);
	return c;
}



//uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
//uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
//uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
//uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
//uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN״̬����


//uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
//uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
//uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
//uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
//uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    OLED_Init();
    
    //EEPROM��ʼ��
    Store_Init(); 
    
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&receive_dat, 1); // ���������ж�
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&receive_dat1, 1); // ���������ж�
    
    HAL_TIM_Base_Start_IT(&htim3);
    //��ʼ��ָ��ģ��
    PS_StaGPIO_Init();
    
    //���flash����
//    Store_Clear();
//    Del_FR_Lib();

    OLED_CLS();
    
    //��ʼ����
    //main_menu();

    //��ʾ��������͹���Ա����
//      for(uint16_t i=1;i<19;i++)
//      {
//          uint16_t value1 = MyFLASH_ReadHalfWord(STORE_START_ADDRESS+i*2);
//         // uint16_t value2 = MyFLASH_ReadWord(STORE_ADMIN_ADDRESS+(i-1)*2);
//          if(i<7){
//            OLED_ShowNum(0+16*(i-1),0,value1,2,16);
//            //OLED_ShowNum(0+16*(i-1),0,Store_Data[i-1],2,16);
//          }
//          else if(i>=7&&i<13)
//          {
//              OLED_ShowNum(0+16*(i-7),2,value1,2,16);
//          }
//          else if(i==13)
//          {
//              OLED_ShowNum(0+16*(i-13),4,value1,2,16);
//          }
//          else if(i>=14&&i<17)
//          {
//              OLED_ShowNum(0+16*(i-14),6,value1,2,16);
//          }
//      }
//    
    uint8_t res = 1;
    printf("AS608ָ��ģ����Կ�ʼ\r\n");
	res = as608_init();
	if(res == 0)
	{
		printf("AS608ָ��ģ���ʼ���ɹ�\r\n");
	}
	else
		printf("AS608ָ��ģ���ʼ��ʧ��\r\n");
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
       //Add_FR();//����ָ��,Ĭ��IDΪ1
		//press_FR();//ˢָ�Ʋ���
		//Del_FR_Lib();//ɾ��ָ�ƿ�
      
      view_display();
      key_display();
      
	  //SYN_FrameInfo(2, "[v7][m1][t5]��ӭʹ��SYN6288");
      
	  //ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
      
		//���������ù�����ο������ֲ�
	
      //��������
//      SYN_FrameInfo(2,"[v7][m1][t5]����ʧ�ܣ������³���");
//	    HAL_Delay(1000);
//      HAL_Delay(1000);
        
      if(uart2_rx_len!=0)
      {
          int temp=uart2_rx_len;
          HAL_Delay(2);
          if(temp==uart2_rx_len)
              usart_disp();
      } 
//      SYN_FrameInfo(2,"[v7][m1][t5]����ʧ�ܣ������³���");
//	    HAL_Delay(1000);
//      
//      if(uart1_rx_len!=0)
//      {
//          int temp=uart1_rx_len;
//          HAL_Delay(2);
//          if(temp==uart1_rx_len)
//              usart1_disp();
//      } 
      
      //key_show();
      
      //��ʾ��������͹���Ա����
//       for(uint16_t i=1;i<19;i++)
//      {
//          uint16_t value1 = MyFLASH_ReadHalfWord(STORE_START_ADDRESS+i*2);
//         // uint16_t value2 = MyFLASH_ReadWord(STORE_ADMIN_ADDRESS+(i-1)*2);
//          if(i<7){
//            OLED_ShowNum(0+16*(i-1),0,value1,2,16);
//            //OLED_ShowNum(0+16*(i-1),0,Store_Data[i-1],2,16);
//          }
//          else if(i>=7&&i<13)
//          {
//              OLED_ShowNum(0+16*(i-7),2,value1,2,16);
//          }
//          else if(i==13)
//          {
//              OLED_ShowNum(0+16*(i-13),4,value1,2,16);
//          }
//          else if(i>=14&&i<19)
//          {
//             // OLED_ShowNum(0+16*(i-14),6,value1,2,16);
//              OLED_ShowNum(0+16*(i-14),6,value1,2,16);
//          }
//          //OLED_ShowNum(0+16*(i-1),2,value2,2,16);
//      }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void key_show(void)
{
    keynum0=button4_4_Scan();
    if(keynum0!=0)
    OLED_ShowNum(60,4,keynum0,2,16);
    if(keynum0==2)
    {
        Store_Data[1]=7;
            Store_Save();
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
