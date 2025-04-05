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



//uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
//uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
//uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
//uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
//uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN状态命令


//uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
//uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
//uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
//uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
//uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令


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
    
    //EEPROM初始化
    Store_Init(); 
    
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&receive_dat, 1); // 启动接收中断
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&receive_dat1, 1); // 启动接收中断
    
    HAL_TIM_Base_Start_IT(&htim3);
    //初始化指纹模块
    //PS_StaGPIO_Init();
    
    //清空flash缓存
//    Store_Clear();
//    Del_FR_Lib();

    OLED_CLS();
    
    //初始界面
    //main_menu();

    //显示解锁密码和管理员密码
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
    printf("AS608指纹模块测试开始\r\n");
	res = as608_init();
	if(res == 0)
	{
		printf("AS608指纹模块初始化成功\r\n");
	}
	else
		printf("AS608指纹模块初始化失败\r\n");
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
       //Add_FR();//添加指纹,默认ID为1
		//press_FR();//刷指纹测试
		//Del_FR_Lib();//删除指纹库
      
      view_display();
      key_display();
      
	  //SYN_FrameInfo(2, "[v7][m1][t5]欢迎使用SYN6288");
      
	  //选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
      
		//其他不常用功能请参考数据手册
	
      //语音播报
//      SYN_FrameInfo(2,"[v7][m1][t5]解锁失败，请重新尝试");
//	    HAL_Delay(1000);
//      HAL_Delay(1000);
        
      if(uart2_rx_len!=0)
      {
          int temp=uart2_rx_len;
          HAL_Delay(2);
          if(temp==uart2_rx_len)
              usart_disp();
      } 
//      
//      if(uart1_rx_len!=0)
//      {
//          int temp=uart1_rx_len;
//          HAL_Delay(2);
//          if(temp==uart1_rx_len)
//              usart1_disp();
//      } 
      
      //key_show();
      
      //显示解锁密码和管理员密码
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
