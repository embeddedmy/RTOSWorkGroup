
#include <stdint.h>

#ifndef _GLOBAL_H
#define _GLOBAL_H

/*
typedef struct
{
	uint8_t sensorData;
}TSensorRecord;
*/
union uDate{
	uint32_t u32_date;
	struct {
	  uint8_t Weekday;
	  uint8_t Month;
	  uint8_t Date;
	  uint8_t Year;
	}uStructDate;
};

union uTime{
	uint32_t u32_Time;
	struct {
		uint8_t Hours;
		uint8_t Minutes;
		uint8_t Seconds;
		uint8_t TimeFormat;
	}uStructTime;
};

union uDataPacket{
	uint32_t datPacket[4];
	struct {
		float temperature;
		float humidity;
		union uDate Date;
		union uTime Time;
	}uStructDataPacket;
};

typedef struct
{
	union uDataPacket uTempnHumidity;
}TSensorRecord;

#define MAX_RECORD_SIZE (10)

#endif
