//transmit.c
#include "cmsis_os.h"
#include "transmit.h"
#include <string.h>
#include "stm32l0xx_hal.h"
#include "main.h"
#include "data.h"

UART_HandleTypeDef huart2;


TSensorRecord record[MAX_RECORD_SIZE];
uint8_t datasize = 0;
uint8_t SerialBuffer[10];


void PrintToUart(TSensorRecord *pthisSensor, uint8_t datasize)
{
	uint8_t i;
	for(i=0; i<datasize; i++, pthisSensor++)
	{
		//printf("\r\ndata: &d", pthisSensor->sensorData);
		sprintf((char*)&SerialBuffer[0], "\r\n%u",pthisSensor->sensorData);
		HAL_UART_Transmit(&huart2,&SerialBuffer[0],5,200);
		//HAL_UART_Transmit(&huart2,"\r\n100",5,200);
	}
}

uint8_t getFromQueue(TSensorRecord *pthisSensor)
{
	TSensorRecord *data;
	osEvent evt;
	uint8_t i;
	for(i=0; i<MAX_RECORD_SIZE; i++, pthisSensor++)
	{
		evt = osMailGet(dataReadqueue, 0);
		if(evt.status==osEventMail)
		{
			data = evt.value.p;
			pthisSensor->sensorData = data->sensorData;
		    osMailFree(dataReadqueue, data);
		}
		else
		{
			//i++;
			break;
		}
	}

	return i;
}

void TransmitTask(void const * argument)
{
	osEvent evt;

	while(1)
	{

		do
		{
			osMessagePut(dataEventqueue, eDBAction_Read, osWaitForever);
			evt = osMessageGet(ReadEventCompleteQueue, osWaitForever);  // wait for message
			if (evt.status == osEventMessage) {
				//read completed
			}
			datasize = getFromQueue(&record[0]);
			PrintToUart(&record[0], datasize);
		}while(datasize!=0);
		osMessagePut(dataEventqueue, eDBAction_ClearRead, osWaitForever);  // Send Message
		osDelay(5*1000);
	}

}
