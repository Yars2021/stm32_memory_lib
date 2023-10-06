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
	while((dev->StatusRegister1 & 0x01) == SET);

	N25QFLASH_CS_UNSELECT(dev);
}

HAL_StatusTypeDef N25Q_device_init(N25Q_device_t *dev, SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *gpio_type, uint32_t gpio_pin){
	dev->dev_t = N25Q_Memory;
    dev->Interface.gpio_pin = gpio_pin;
    dev->Interface.gpio_type = gpio_type;
	dev->Interface.spi_handle = spi_handle;
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

			N25Q_WriteEnable(dev);
			N25QFLASH_CS_SELECT(dev);
			N25Qxx_Spi(ENTER_4_BYTE_ADDR_MODE_CMD, dev);
			N25QFLASH_CS_UNSELECT(dev);
			N25Q_WriteDisable(dev);
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
			return HAL_ERROR;
	}


	dev->PageSize = 256;
	dev->SubSectorSize = 0x1000;
	dev->SubSectorCount = dev->SectorCount * 16;
	dev->PageCount = (dev->SubSectorCount * dev->SubSectorSize) / dev->PageSize;
	dev->SectorSize = dev->SubSectorSize * 16;
	dev->CapacityInKiloByte = (dev->SubSectorCount * dev->SubSectorSize) / 1024;

	dev->Lock = 0;
	return HAL_OK;
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

HAL_StatusTypeDef N25Qxx_WritePage(uint8_t *buff, uint32_t addr, uint32_t len_up_to_PageSize, N25Q_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	N25Qxx_WaitForWriteEnd(dev);

	N25Q_WriteEnable(dev);

	N25QFLASH_CS_SELECT(dev);
	int addrlen = 3;
	if(dev->device_model >= N25Q512){
		N25Qxx_Spi(PAGE_PROG_4_BYTE_ADDR_CMD, dev);
		N25Qxx_Spi((addr & 0xFF000000) >> 24, dev);
	}
	else N25Qxx_Spi(PAGE_PROG_CMD, dev);

	/*!< Send WriteAddr high nibble address byte to write to */
	N25Qxx_Spi((addr & 0xFF0000) >> 16, dev);
	/*!< Send WriteAddr medium nibble address byte to write to */
	N25Qxx_Spi((addr & 0xFF00) >> 8, dev);
	/*!< Send WriteAddr low nibble address byte to write to */
	N25Qxx_Spi(addr & 0xFF, dev);

	for(int i=0; i<len_up_to_PageSize; i++){
		N25Qxx_Spi(buff[i], dev);
	}

	N25QFLASH_CS_UNSELECT(dev);

	N25Qxx_WaitForWriteEnd(dev);

	N25Q_WriteDisable(dev);

	HAL_Delay(1);
	dev->Lock = 0;
	return HAL_OK;
}

HAL_StatusTypeDef N25Q_writemem(N25Q_device_t *dev, uint8_t *buff, int len, size_t waddr){
    if(len > dev->SectorCount * dev->SectorSize){
        return HAL_ERROR;
    }
	HAL_Delay(100);

	HAL_StatusTypeDef status = HAL_OK;

	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  	Addr = waddr % dev->PageSize;
  	count = dev->PageSize - Addr;
  	NumOfPage =  len / dev->PageSize;
  	NumOfSingle = len % dev->PageSize;

    if (Addr == 0) /*!< waddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< len < sFLASH_PAGESIZE */
    {
      status |= N25Qxx_WritePage(buff, waddr, len, dev);
    }
    else /*!< len > sFLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        status |=N25Qxx_WritePage(buff, waddr, dev->PageSize, dev);
        waddr +=  dev->PageSize;
        buff += dev->PageSize;
      }

      status |=N25Qxx_WritePage(buff, waddr, NumOfSingle, dev);
    }
  }
  else /*!< waddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< len < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (len + waddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        status |= N25Qxx_WritePage(buff, waddr, count, dev);
        waddr +=  count;
        buff += count;

        status |= N25Qxx_WritePage(buff, waddr, temp, dev);
      }
      else
      {
        status |= N25Qxx_WritePage(buff, waddr, len, dev);
      }
    }
    else /*!< len > sFLASH_PAGESIZE */
    {
      len -= count;
      NumOfPage =  len / dev->PageSize;
      NumOfSingle = len % dev->PageSize;

      status |= N25Qxx_WritePage(buff, waddr, count, dev);
      waddr +=  count;
      buff += count;

      while (NumOfPage--)
      {
        status |= N25Qxx_WritePage(buff, waddr, dev->PageSize, dev);
        waddr +=  dev->PageSize;
        buff += dev->PageSize;
      }

      if (NumOfSingle != 0)
      {
        status |= N25Qxx_WritePage(buff, waddr, NumOfSingle, dev);
      }
    }
  }
    return status;
}

HAL_StatusTypeDef N25Q_readmem(N25Q_device_t *dev, uint8_t *buff, size_t len, size_t addr){
    while(dev->Lock == 1)
	HAL_Delay(100);

	HAL_StatusTypeDef status = HAL_OK;
	dev->Lock = 1;

	N25QFLASH_CS_SELECT(dev);
	int addrlen = 3;
	if(dev->device_model >= N25Q512){
		N25Qxx_Spi(READ_4_BYTE_ADDR_CMD, dev);
		N25Qxx_Spi((addr & 0xFF000000) >> 24, dev);
	}
	else N25Qxx_Spi(READ_CMD, dev);
	/*!< Send WriteAddr high nibble address byte to write to */
	N25Qxx_Spi((addr & 0xFF0000) >> 16, dev);
	/*!< Send WriteAddr medium nibble address byte to write to */
	N25Qxx_Spi((addr & 0xFF00) >> 8, dev);
	/*!< Send WriteAddr low nibble address byte to write to */
	N25Qxx_Spi(addr & 0xFF, dev);

	for(int i=0; i<len; i++){
		buff[i] = N25Qxx_Spi(0, dev);
	}

	N25QFLASH_CS_UNSELECT(dev);

	HAL_Delay(1);
	dev->Lock = 0;
    return status;
}

void N25Q_reset(N25Q_device_t *dev){
	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(RESET_ENABLE_CMD, dev);
	N25QFLASH_CS_UNSELECT(dev);
	N25QFLASH_CS_SELECT(dev);
	N25Qxx_Spi(RESET_MEMORY_CMD, dev);
	N25QFLASH_CS_UNSELECT(dev);
}