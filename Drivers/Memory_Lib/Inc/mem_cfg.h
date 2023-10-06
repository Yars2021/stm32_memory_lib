#ifndef _MEM_CFG_
#define _MEM_CFG_

// Enable memory types
#define FLASH_MEM
// #define EEPROM_MEM
#define N25Q_MEM

// End address of the flash restricted zone
#define FLASH_RESTRICTED_ADDR 0x08000000   

// // I2C NAME
// #define I2C_HANDLER_1 hi2c1

// // SPI NAME
#define SPI_HANDLER_1 spi

// // Config EEPROM devices (from 0 to 3)
// #define EEPROM_DEV_0 AT24C256, & I2C_HANDLER_1, 0x010

// // Config N25Q devices (from 0 to 3)
#define N25Q_DEV_0 & SPI_HANDLER_1, GPIOB, GPIO_PIN_12

#endif /* _MEM_CFG_ */