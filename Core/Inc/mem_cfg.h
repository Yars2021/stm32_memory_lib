#ifndef _MEM_CFG_
#define _MEM_CFG_

// Enable memory types
#define FLASH_MEM
//#define EEPROM_MEM
#define W25X_MEM

// Config EEPROM devices (from 0 to 3)
//#define EEPROM_DEV_0 AT24C256, &hi2c1, 0x010

// Config W25x devices (from 0 to 3)
#define W25X_DEV_0 &spi, GPIOB, GPIO_PIN_12

#endif /* _MEM_CFG_ */