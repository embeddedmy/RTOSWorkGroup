//data.h
#define FLASH_USER_START_ADDR   0x08080000   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     0x080807FF   /* End @ of user Flash area */

#define FLASH_USER_STORE_POINTER	0x08080708

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



