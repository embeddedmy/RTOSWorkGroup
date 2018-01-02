//sensor.c
#include "cmsis_os.h"
#include "sensor.h"
#include "main.h"
#include "data.h"
#include "stm32l0xx_hal.h"
#include "HTS221.h"

//osMailQId dataWriteQueue;

I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

void send_to_queue(TSensorRecord *pthisSensor)
{
	//TODO: push to write queue
	osMailPut(dataWritequeue, pthisSensor);
	osMessagePut(dataEventqueue, eDBAction_Write, osWaitForever);  // Send Message
}

void SetSensorData(TSensorRecord* data)
{
	RTC_TimeTypeDef Time;

	data->uTempnHumidity.uStructDataPacket.temperature = (float)HTS221_GetTemperature();
	data->uTempnHumidity.uStructDataPacket.humidity = (float)HTS221_GetHumidity();
	HAL_RTC_GetDate(&hrtc,(RTC_DateTypeDef *)(&data->uTempnHumidity.uStructDataPacket.Date.uStructDate.Weekday),RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc,&Time,RTC_FORMAT_BIN );
	data->uTempnHumidity.uStructDataPacket.Time.uStructTime.Hours = Time.Hours;
	data->uTempnHumidity.uStructDataPacket.Time.uStructTime.Minutes = Time.Minutes;
	data->uTempnHumidity.uStructDataPacket.Time.uStructTime.Seconds= Time.Seconds;
	data->uTempnHumidity.uStructDataPacket.Time.uStructTime.TimeFormat= Time.TimeFormat;
}

void SensorTask(void const * argument)
{
	TSensorRecord* data;
	HTS221_Init(&hi2c1);

	while(1)
	{
		data = osMailAlloc(dataWritequeue, 0);
		SetSensorData(data);
		send_to_queue(data);
		osDelay(1000);
	}
}

