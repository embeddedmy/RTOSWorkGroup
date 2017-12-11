//data.h
#define FLASH_USER_START_ADDR   0x8008000   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     0x800A000   /* End @ of user Flash area */

#define EEPROM_RECORD_SIZE	(50)

typedef struct
{
	uint8_t Prod;
	uint8_t Cons;
}TEEPROMFFIFO;

enum
{
	eDBAction_Write,
	eDBAction_Read,
	eDBAction_ClearRead,
	eDBAction_ClearAll
}eDBTaskAction;



