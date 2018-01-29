#include "ResetFunction.h"

#define BUTTON_TIMEOUT						(uint32_t)0x1388		// 5 seconds timeout
static int32_t timeout = 0;

osThreadId Task_ManagEeprom;
osMutexId MutexEeprom;
osSemaphoreId SemaEepromManager;



static void task_EepromManager ( void const* argument );



void ResetTaskInit (void){
	osThreadDef(EepromManager, task_EepromManager, osPriorityHigh, 0 , 128);
	Task_ManagEeprom = osThreadCreate(osThread(EepromManager),NULL);

	osSemaphoreDef(SemaDefEeprom);
	SemaEepromManager = osSemaphoreCreate(osSemaphore(SemaDefEeprom), 1);
	osSemaphoreWait(SemaEepromManager,osWaitForever);

	osMutexDef(MutexEeprom);
	MutexEeprom = osMutexCreate(osMutex(MutexEeprom));
}


static void task_EepromManager ( void const* argument ){
	uint8_t i;

	for (;;){

		osSemaphoreWait(SemaEepromManager,osWaitForever);

		for (i=0; i<3 ; i++){
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
				HAL_Delay(100);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
				HAL_Delay(100);
			}
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);

			//TODO: Implement Mutex before EEPROM Clean is carried out
			osMutexWait(MutexEeprom, osWaitForever);

			osMessagePut(dataEventqueue, eDBAction_ClearAll, osWaitForever);
//			EEPROM_Clean();

			osMutexRelease(MutexEeprom);

			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);

	}
}



/**************************************************
 *
 *   Interrupt routine to release the Semaphore
 *
 *
 *
 * **************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if (timeout!= 0){
		timeout = osKernelSysTick() - timeout;
		if (timeout >= BUTTON_TIMEOUT){
			osSemaphoreRelease(SemaEepromManager);
		}
		timeout = 0;
	}
	else{
		timeout = osKernelSysTick();
	}
}
