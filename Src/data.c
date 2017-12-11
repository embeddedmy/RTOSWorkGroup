//data.c

#include "cmsis_os.h"
#include "stm32l0xx_hal.h"
#include "data.h"
#include "global.h"
#include "main.h"

TEEPROMFFIFO EEPROMFifo;
uint8_t tempCons;
UART_HandleTypeDef huart2;
uint8_t databuffer[10];

void EEPROM_Erase(uint32_t addr, uint32_t len)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError = 0;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = FLASH_USER_START_ADDR + addr;
	EraseInitStruct.NbPages = len/FLASH_PAGE_SIZE + 1;

	HAL_FLASH_Unlock();
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		//error process
	}
	HAL_FLASH_Lock();

}

void EEPROM_Write(uint32_t addr, uint8_t* dat, uint32_t len)
{
	uint32_t i;
	HAL_FLASH_Unlock();
	for(i=0; i<len/4; i++)
	{
	    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + addr, *(uint32_t*)dat) == HAL_OK)
	    {
	    	dat = dat + 4;
	    	addr = addr + 4;
	    }
	}
	HAL_FLASH_Lock();
}

void EEPROM_Read(uint32_t addr, uint8_t* dat, uint32_t len)
{
	uint32_t i;
	for(i=0; i<len/4; i++)
	{
		*dat = *(__IO uint8_t*)(FLASH_USER_START_ADDR + addr);
		addr++;
		dat++;
	}
}

void WriteData(TSensorRecord* pRecord, uint8_t* prod)
{
	uint32_t tempdata = pRecord->sensorData;
	EEPROM_Write((*prod)*4, &tempdata, 4);
	(*prod)++;
	if((*prod)>=EEPROM_RECORD_SIZE)
	{
		(*prod)=0;
	}
	if((*prod) == tempCons)
	{
		tempCons++;
	}
	
	if((*prod) == EEPROMFifo.Cons)
	{
		EEPROMFifo.Cons++;
	}
}

uint8_t ReadData(uint8_t* cons, TSensorRecord* dataRead)
{
	uint32_t tempdata;

	if(*cons != EEPROMFifo.Prod)
	{
		EEPROM_Read((*cons)*4, &tempdata, 4);
		dataRead->sensorData = (uint8_t)tempdata;
		(*cons)++;
		if(*cons>=EEPROM_RECORD_SIZE)
		{
			*cons=0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void DBTask(void const * argument)
{
	osEvent evt;
	TSensorRecord tempdata;
	TSensorRecord* data;
	uint32_t Action = 10;
	uint32_t savedata= 50;
	uint8_t i;

	while(1)
	{
		evt = osMessageGet(dataEventqueue, osWaitForever);  // wait for message
		if (evt.status == osEventMessage) {
			Action = evt.value.v;
		}

		switch(Action)
		{
			case eDBAction_Write:
				HAL_UART_Transmit(&huart2,"write",sizeof("write"),200);
				while(1)
				{		
					evt = osMailGet(dataWritequeue, 0);
					if(evt.status==osEventMail)
					{
						data = evt.value.p;
						WriteData(data, &EEPROMFifo.Prod);
					    osMailFree(dataWritequeue, data);
					}
					else
					{
						break;
					}
				}
			break;
			case eDBAction_Read:
				HAL_UART_Transmit(&huart2,"read",sizeof("read"),200);
				//tempCons = EEPROMFifo.Cons;
				while(1)
				{
					if(ReadData(&tempCons,&tempdata))
					{
						data = osMailAlloc(dataReadqueue, 0);
						data->sensorData = tempdata.sensorData;
						osMailPut(dataReadqueue, data);
					}
					else
					{
						break;
					}
				}
				osMessagePut(ReadEventCompleteQueue, 1, osWaitForever);
			break;
			case eDBAction_ClearRead:
				HAL_UART_Transmit(&huart2,"clearRead",sizeof("clearRead"),200);
				EEPROMFifo.Cons = tempCons;
			break;
			case eDBAction_ClearAll:
				HAL_UART_Transmit(&huart2,"clearAll",sizeof("clearAll"),200);
				EEPROMFifo.Prod = 0;
				EEPROMFifo.Cons = 0;
				tempCons = 0;
				EEPROM_Erase(0, 256);
			break;
		}
	}
}

