/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

IRDA_HandleTypeDef hirda1;
UART_HandleTypeDef huart2;



//osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t thread1_counter;
uint32_t Queue_number;

osThreadId Task_NVM_ReadWrite;
osThreadId Task_UartSend;
osThreadId Task_SensorRead;
osThreadId Task_HearBeatLED;
osThreadId Task_ErrorCheck;


osMessageQId Queue_SendTo_TaskNVM;
osMessageQId Queue_SendTo_Uart;

QueueSetHandle_t NVM_QueueSet;

typedef enum{
	EV_ReadsFromTempSensor,
	EV_ReturnFromUartSend,
	EV_ReturnFromBlinkLED,
}EventId_NVM_Check;


typedef struct{
	uint16_t temperature;
	uint16_t humidity;
//	uint8_t * ptr_sent_Success;
}QMessageData_USART;

typedef struct{
	EventId_NVM_Check EventId;
	uint8_t dataArray[4];
	uint8_t* ptr_sent_Success;
}QMessageData_ForNVM;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_IRDA_Init(void);
static void MX_I2C1_Init(void);
//void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void func_NVM_Manager(void const* argument);
void func_LEDBlink (void const* argument);
void func_UartPrint (void const* argument);
void func_SensorRead (void const* argument);
void vApplicationTickHook(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_USART1_IRDA_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
//  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
//  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFE);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  osThreadDef(HeartBeat, func_LEDBlink, osPriorityNormal, 0 , 64);
  Task_HearBeatLED = osThreadCreate(osThread(HeartBeat),NULL);

  osThreadDef(HTS221_sensor, func_SensorRead, osPriorityNormal, 0, 64);
  Task_SensorRead = osThreadCreate(osThread(HTS221_sensor),NULL);

  osThreadDef(NVM, func_NVM_Manager, osPriorityAboveNormal, 0 ,128);
  Task_NVM_ReadWrite = osThreadCreate(osThread(NVM),NULL);

  osThreadDef(UartPrint, func_UartPrint, osPriorityAboveNormal, 0 , 128);
  Task_UartSend = osThreadCreate(osThread(UartPrint),NULL);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */



//  osMessageQDef(NVM_Manager,1,QMessageData_ForNVM);
//  osMessageQDef(NVM_Manager,1,uint8_t);
//  Queue_SendTo_TaskNVM = osMessageCreate(osMessageQ(NVM_Manager),Task_NVM_ReadWrite);

//  osMessageQDef(QSensor, 1, QMessageData_Sensor);
//  osMessageQDef(QSensor, 1, uint8_t);
//  Queue_SendTo_Uart = osMessageCreate(osMessageQ(QSensor), Task_UartSend);

  Queue_SendTo_TaskNVM = xQueueCreate(1,sizeof(QMessageData_ForNVM));
  Queue_SendTo_Uart = xQueueCreate(1,sizeof(QMessageData_USART));
//
//  xQueueCreateSet(1*sizeof(QMessageData_ForNVM)+1*sizeof(QMessageData_Sensor));
//  xQueueAddToSet(Queue_SendTo_TaskNVM,NVM_QueueSet);

  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 3, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }

}

/* USART1 init function */
static void MX_USART1_IRDA_Init(void)
{

  hirda1.Instance = USART1;
  hirda1.Init.BaudRate = 115200;
  hirda1.Init.WordLength = IRDA_WORDLENGTH_7B;
  hirda1.Init.Parity = IRDA_PARITY_NONE;
  hirda1.Init.Mode = IRDA_MODE_TX_RX;
  hirda1.Init.Prescaler = 10;
  hirda1.Init.PowerMode = IRDA_POWERMODE_NORMAL;
  if (HAL_IRDA_Init(&hirda1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void func_NVM_Manager(void const* argument){
	QMessageData_ForNVM Rawdata;
	QMessageData_USART Uartdata;
	uint8_t Uart_Updatecounter=0;
	for(;;){
//			TODO: Copy data from Queue then write into the EEPROM.
		xQueueReceive(Queue_SendTo_TaskNVM,&Rawdata,portMAX_DELAY);
		switch (Rawdata.EventId) {
			case EV_ReturnFromBlinkLED:
				break;

			case EV_ReadsFromTempSensor:
//			TODO: Receive the data from Temperature Sensor, then Save it into the NVM
//				write to EEPROM; start Counter
				if (Uart_Updatecounter>5){
					xQueueSend(Queue_SendTo_Uart,&Uartdata,100);
					Uart_Updatecounter = 0;
				}
				else {
//					Need to write into EEPROM then increment Uart_Updatecounter
					Uart_Updatecounter++;
				}
				break;

			case EV_ReturnFromUartSend:
//			TODO: After confirm receive successful (0x01),
//				delete data in NVM.
//				else resend the data in NVM to UART.
				if (Rawdata.dataArray[0]==0x00){

				}
				else {

				}
				break;

			default:
				break;
		}
	}
}


void func_LEDBlink (void const* argument){
	uint32_t xLastTickWakeup = osKernelSysTick();
	thread1_counter = 0;
//	uint8_t TestSentPtr = 0;
//	QMessageData_ForNVM toNVM;
//	toNVM.ptr_sent_Success = &TestSentPtr;
	for(;;)
	{
		thread1_counter++;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		osDelayUntil(&xLastTickWakeup,1000);
//		toNVM.EventId = EV_ReturnFromBlinkLED;
//		toNVM.dataArray[0]=0xBC;
//		*toNVM.ptr_sent_Success = 0x04;
//		xQueueSend(Queue_SendTo_TaskNVM,&toNVM,100);
	}
}


void func_UartPrint (void const* argument){
	QMessageData_ForNVM sendReturn;
	QMessageData_USART receiveData;
	for(;;)
	{
		xQueueReceive(Queue_SendTo_Uart,&receiveData,portMAX_DELAY);
		HAL_UART_Transmit(&huart2,(uint8_t *)"Next Game",10,200);
		sendReturn.EventId = EV_ReturnFromUartSend;
		sendReturn.dataArray[0]=0x00;
		xQueueSend(Queue_SendTo_TaskNVM,&sendReturn,100);
	}
}


void func_SensorRead (void const* argument){
	QMessageData_ForNVM data;
	uint8_t I2C_Buffer[5];
//	= {0b10000111,0b00000000,0b00000000,}

	HAL_I2C_Mem_Write(&hi2c1,0xBE,0x20,I2C_MEMADD_SIZE_8BIT,I2C_Buffer,3,100);
	for(;;)
	{
		HAL_I2C_Mem_Read(&hi2c1,0xBE,0x0F,I2C_MEMADD_SIZE_8BIT,&I2C_Buffer[0],1,100);
		HAL_I2C_Mem_Read(&hi2c1,0xBE,0x27,I2C_MEMADD_SIZE_8BIT,&I2C_Buffer[1],1,100);
		HAL_I2C_Mem_Read(&hi2c1,0xBE,0x28,I2C_MEMADD_SIZE_8BIT,&I2C_Buffer[3],4,100);

		data.EventId = EV_ReadsFromTempSensor;
		data.dataArray[0] = I2C_Buffer[0];
		data.dataArray[1] = I2C_Buffer[1];
		data.dataArray[2] = I2C_Buffer[2];
		data.dataArray[4] = I2C_Buffer[3];

		xQueueSend(Queue_SendTo_TaskNVM,&data,100);

		osDelay(1000);
	}
}

void vApplicationTickHook (void){
	for(;;){

	}
}


/* USER CODE END 4 */

/* StartDefaultTask function */
//void StartDefaultTask(void const * argument)
//{
//
//  /* USER CODE BEGIN 5 */
////   Infinite loop
//  for(;;)
//  {
//	  thread1_counter++;
//	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
////	  HAL_UART_Transmit(&huart2,(uint8_t *)"Next Game",10,200);
//	  osDelay(1000);
////	  xQueueReceive(queue2,&receivebuffer,portMAX_DELAY);
////	  osMessageGet(queue1,2000);
//  }
//  /* USER CODE END 5 */
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/