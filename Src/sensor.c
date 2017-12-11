//sensor.c
#include "cmsis_os.h"
#include "sensor.h"
#include "main.h"
#include "data.h"
//osMailQId dataWriteQueue;

void send_to_queue(TSensorRecord *pthisSensor)
{
	//TODO: push to write queue
	osMailPut(dataWritequeue, pthisSensor);
}

void SensorTask(void const * argument)
{
	TSensorRecord* data;
	static uint8_t counter = 00;

	while(1)
	{
		data = osMailAlloc(dataWritequeue, 0);
		data->sensorData = counter;
		counter++;
		send_to_queue(data);
		osMessagePut(dataEventqueue, eDBAction_Write, osWaitForever);  // Send Message
		osDelay(1000);
	}
}
