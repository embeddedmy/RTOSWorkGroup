//transmit.c
#include "cmsis_os.h"
#include "transmit.h"
#include <string.h>
#include "stm32l0xx_hal.h"
#include "main.h"
#include "data.h"
#include <stdlib.h>

UART_HandleTypeDef huart2;


TSensorRecord record[MAX_RECORD_SIZE];
uint8_t datasize = 0;
char OutputText[90]="";
char tempText[5] = "";

void PrintToUart(TSensorRecord *pthisSensor, uint8_t datasize)
{
	uint8_t i;
	for(i=0; i<datasize; i++, pthisSensor++)
	{
		//sprintf((char*)&SerialBuffer[0], "\r\n%u",pthisSensor->sensorData);
		//HAL_UART_Transmit(&huart2,&SerialBuffer[0],5,200);
		memset(OutputText,0x00, 90);
		strcat(OutputText,"Current Temperature is ");
		itoa(pthisSensor->uTempnHumidity.uStructDataPacket.temperature,tempText,10);
		strcat(OutputText,tempText);
		strcat(OutputText, " Celsius\n");

		strcat(OutputText, "Humidity is ");
		itoa(pthisSensor->uTempnHumidity.uStructDataPacket.humidity,tempText,10);
		strcat(OutputText,tempText);
		strcat(OutputText, " %rh\n");

		strcat(OutputText, " @ Time ");

		itoa(pthisSensor->uTempnHumidity.uStructDataPacket.Time.uStructTime.Hours,tempText,10);
		strcat(OutputText,tempText);
		strcat(OutputText,":");
		itoa(pthisSensor->uTempnHumidity.uStructDataPacket.Time.uStructTime.Minutes,tempText,10);
		strcat(OutputText,tempText);
		strcat(OutputText,":");
		itoa(pthisSensor->uTempnHumidity.uStructDataPacket.Time.uStructTime.Seconds,tempText,10);
		strcat(OutputText,tempText);
		strcat(OutputText, "\n \n");

		HAL_UART_Transmit(&huart2,(uint8_t *)OutputText,sizeof(OutputText),200);
	}
}

uint8_t getFromQueue(TSensorRecord *pthisSensor)
{
	TSensorRecord *data;
	osEvent evt;
	uint8_t i;
	for(i=0; i<MAX_RECORD_SIZE; i++)
	{
		evt = osMailGet(dataReadqueue, 0);
		if(evt.status==osEventMail)
		{
			data = evt.value.p;
			memcpy(&pthisSensor[i], data, sizeof(TSensorRecord));
			//pthisSensor->sensorData = data->sensorData;
		    osMailFree(dataReadqueue, data);
		}
		else
		{
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
			osMessagePut(dataEventqueue, eDBAction_Read, osWaitForever);    //cheng: i would think a short timeout is better, instead of osWaitForever, avoid permanent blocking, changing the 5seconds interval
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
