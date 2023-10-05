#include "N25Q_mem.h"

uint8_t	N25Qxx_Spi(uint8_t	Data, N25Q_device_t *dev)
{
	uint8_t	ret;

	HAL_SPI_TransmitReceive(dev->Interface.spi_handle, &Data, &ret, 1, 100);

	return ret;
}

void N25QFLASH_CS_SELECT(N25Q_device_t *dev){
    HAL_GPIO_WritePin(dev->Interface.gpio_type, dev->Interface.gpio_pin, GPIO_PIN_RESET);
    return;
}

void N25QFLASH_CS_UNSELECT(N25Q_device_t *dev){
    HAL_GPIO_WritePin(dev->Interface.gpio_type, dev->Interface.gpio_pin, GPIO_PIN_SET);
    return;
}

void N25Q_WriteEnable(N25Q_device_t *dev)
{
	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(WRITE_ENABLE_CMD, dev);
	N25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(1);
}
void N25Q_WriteDisable(N25Q_device_t *dev)
{
	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(WRITE_DISABLE_CMD, dev);
	N25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(1);
}

uint32_t N25Qxx_ReadID(N25Q_device_t *dev)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	N25QFLASH_CS_SELECT(dev);

	N25Qxx_Spi(READ_ID_CMD, dev);

	Temp0 = N25Qxx_Spi(0, dev);
	Temp1 = N25Qxx_Spi(0, dev);
	Temp2 = N25Qxx_Spi(0, dev);

	N25QFLASH_CS_UNSELECT(dev);(dev);

	return Temp2;
}

void N25Qxx_WaitForWriteEnd(N25Q_device_t *dev)
{
	HAL_Delay(1);
	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(READ_STATUS_REG_CMD, dev);

	do{
		dev->StatusRegister1 = N25Qxx_Spi(0, dev);
		HAL_Delay(1);
	}
	while((dev->StatusRegister1 & 0x01) == 0x01);

	N25QFLASH_CS_UNSELECT(dev);(dev);
}

HAL_StatusTypeDef N25Q_device_init(N25Q_device_t *dev, SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *gpio_type, uint32_t gpio_pin){
	dev->dev_t = N25Q_Memory;
    dev->Interface.gpio_pin = gpio_pin;
    dev->Interface.gpio_type = gpio_type;
    dev->Lock = 1;
	while(HAL_GetTick() < 100)
	HAL_Delay(1);

	N25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(100);

	uint32_t id;
	
	id = N25Qxx_ReadID(dev);

	switch(id)
	{
		case 0x20:	// 	N25Q512
			dev->device_model = N25Q512;
			dev->SectorCount = 1024;
		break;

		case 0x10:	// 	N25Q256
			dev->device_model = N25Q256;
			dev->SectorCount = 512;
		break;

		case 0x8:	// 	N25Q128
			dev->device_model = N25Q128;
			dev->SectorCount = 256;
		break;


		default:

			dev->Lock = 0;
			return 0;
	}


	dev->PageSize = 256;
	dev->SectorSize = 0x1000;
	dev->SubSectorCount = dev->SectorCount * 16;
	dev->PageCount = (dev->SubSectorCount * dev->SubSectorSize) / dev->PageSize;
	dev->SectorSize = dev->SubSectorSize * 16;
	dev->CapacityInKiloByte = (dev->SubSectorCount * dev->SubSectorSize) / 1024;

	dev->Lock = 0;
	return 1;

}

void N25Qxx_EraseChip(N25Q_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	N25Q_WriteEnable(dev);

	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(BULK_ERASE_CMD , dev);
	N25QFLASH_CS_UNSELECT(dev);(dev);

	N25Qxx_WaitForWriteEnd(dev);
	N25Q_WriteDisable(dev);

	HAL_Delay(10);

	dev->Lock = 0;
}

void N25Qxx_EraseSector(uint32_t SectorAddr, N25Q_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	N25Qxx_WaitForWriteEnd(dev);
	SectorAddr = SectorAddr * dev->SectorSize;

	N25Q_WriteEnable(dev);

	N25QFLASH_CS_SELECT(dev);
	if(dev->device_model >= N25Q512) N25Qxx_Spi(SECTOR_ERASE_4_BYTE_ADDR_CMD, dev);
	else N25Qxx_Spi(SECTOR_ERASE_CMD, dev);
	
	N25Qxx_Spi(SectorAddr, dev);

	N25QFLASH_CS_UNSELECT(dev);

	N25Qxx_WaitForWriteEnd(dev);
	N25Q_WriteDisable(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

void N25Qxx_EraseSubSector(uint32_t SubSectorAddr, N25Q_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	N25Qxx_WaitForWriteEnd(dev);

	SubSectorAddr = SubSectorAddr * dev->SubSectorSize;

	N25Q_WriteEnable(dev);

	N25QFLASH_CS_SELECT(dev);

	N25QFLASH_CS_SELECT(dev);
	if(dev->device_model >= N25Q512) N25Qxx_Spi(SUBSECTOR_ERASE_4_BYTE_ADDR_CMD, dev);
	else N25Qxx_Spi(SUBSECTOR_ERASE_CMD, dev);
	
	N25Qxx_Spi(SubSectorAddr, dev);

	N25QFLASH_CS_UNSELECT(dev);

	N25Qxx_WaitForWriteEnd(dev);
	N25Q_WriteDisable(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

uint32_t N25Qxx_PageToSector(uint32_t PageAddress, N25Q_device_t *dev)
{
	return((PageAddress * dev->PageSize) / dev->SectorSize);
}

uint32_t N25Qxx_PageToSubSector(uint32_t PageAddress, N25Q_device_t *dev)
{
	return((PageAddress * dev->PageSize) / dev->SubSectorSize);
}


uint32_t N25Qxx_SectorToSubSector(uint32_t SectorAddress, N25Q_device_t *dev)
{
	return((SectorAddress * dev->SectorSize) / dev->SubSectorSize);
}


uint32_t N25Qxx_SectorToPage(uint32_t SectorAddress, N25Q_device_t *dev)
{
	return(SectorAddress * dev->SectorSize) / dev->PageSize;
}


uint32_t N25Qxx_SubSectorToPage(uint32_t BlockAddress, N25Q_device_t *dev)
{
	return (BlockAddress * dev->SubSectorSize) / dev->PageSize;
}

void N25Qxx_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize, N25Q_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	if(((NumByteToWrite_up_to_PageSize + OffsetInByte) > dev->PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = dev->PageSize - OffsetInByte;

	if((OffsetInByte + NumByteToWrite_up_to_PageSize) > dev->PageSize)
		NumByteToWrite_up_to_PageSize = dev->PageSize - OffsetInByte;


	N25Qxx_WaitForWriteEnd(dev);

	N25QFLASH_CS_SELECT(dev);

	if(dev->device_model >= N25Q512)
		N25Qxx_Spi(PAGE_PROG_4_BYTE_ADDR_CMD, dev);
	else N25Qxx_Spi(PAGE_PROG_CMD, dev);

	Page_Address = (Page_Address * dev->PageSize) + OffsetInByte;

	N25Qxx_Spi(Page_Address, dev);

	HAL_SPI_Transmit(dev->Interface.spi_handle, pBuffer, NumByteToWrite_up_to_PageSize, 100);

	N25QFLASH_CS_UNSELECT(dev);

	N25Qxx_WaitForWriteEnd(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

HAL_StatusTypeDef N25Q_writemem(N25Q_device_t *dev, uint8_t *buff, size_t len, size_t addr){
    if(len > dev->SectorCount * dev->SectorSize){
        return HAL_ERROR;
    }
	N25Q_WriteEnable(dev);

    while(len > 0){
        N25Qxx_WritePage(buff, addr/dev->PageSize, addr%dev->PageSize, len, dev);
        len-= dev->PageSize;
        addr += dev->PageSize;
        addr -= (addr%dev->PageSize);
        buff += dev->PageSize;
    }
	N25Q_WriteDisable(dev);
    return HAL_OK;
}

HAL_StatusTypeDef N25Q_readmem(N25Q_device_t *dev, uint8_t *buff, size_t len, size_t addr){
    while(dev->Lock == 1)
	HAL_Delay(1);

	HAL_StatusTypeDef status = HAL_OK;
	dev->Lock = 1;

	N25QFLASH_CS_SELECT(dev);
	if(dev->device_model >= N25Q512)
		N25Qxx_Spi(READ_4_BYTE_ADDR_CMD, dev);
	else N25Qxx_Spi(READ_CMD, dev);

	N25Qxx_Spi(addr, dev);

	status |= HAL_SPI_Receive(dev->Interface.spi_handle, buff, len, 2000);

	N25QFLASH_CS_UNSELECT(dev);

	HAL_Delay(1);
	dev->Lock = 0;
    return status;
}