#include "w25x_mem.h"

uint8_t	W25qxx_Spi(uint8_t	Data, W25x_device_t *dev)
{
	uint8_t	ret;

	HAL_SPI_TransmitReceive(dev->Interface.spi_handle, &Data, &ret, 1, 100);

	return ret;
}

void W25QFLASH_CS_SELECT(W25x_device_t *dev){
    HAL_GPIO_WritePin(dev->Interface.gpio_type, dev->Interface.gpio_pin, GPIO_PIN_RESET);
    return;
}

void W25QFLASH_CS_UNSELECT(W25x_device_t *dev){
    HAL_GPIO_WritePin(dev->Interface.gpio_type, dev->Interface.gpio_pin, GPIO_PIN_SET);
    return;
}

void W25x_WriteEnable(W25x_device_t *dev)
{
	W25QFLASH_CS_SELECT(dev);
	W25qxx_Spi(W25_WRITE_ENABLE, dev);
	W25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(1);
}
void W25x_WriteDisable(W25x_device_t *dev)
{
	W25QFLASH_CS_SELECT(dev);
	W25qxx_Spi(W25_WRITE_DISABLE, dev);
	W25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(1);
}

uint32_t W25qxx_ReadID(W25x_device_t *dev)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_GET_JEDEC_ID, dev);

	Temp0 = W25qxx_Spi(W25QXX_DUMMY_BYTE, dev);
	Temp1 = W25qxx_Spi(W25QXX_DUMMY_BYTE, dev);
	Temp2 = W25qxx_Spi(W25QXX_DUMMY_BYTE, dev);

	W25QFLASH_CS_UNSELECT(dev);(dev);

	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

void W25qxx_WaitForWriteEnd(W25x_device_t *dev)
{
	HAL_Delay(1);
	W25QFLASH_CS_SELECT(dev);
	W25qxx_Spi(W25_READ_STATUS_1, dev);

	do{
		dev->StatusRegister1 = W25qxx_Spi(W25QXX_DUMMY_BYTE, dev);
		HAL_Delay(1);
	}
	while((dev->StatusRegister1 & 0x01) == 0x01);

	W25QFLASH_CS_UNSELECT(dev);(dev);
}

HAL_StatusTypeDef w25x_device_init(W25x_device_t *dev, SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *gpio_type, uint32_t gpio_pin){
    dev->Interface.gpio_pin = gpio_pin;
    dev->Interface.gpio_type = gpio_type;
    dev->Lock = 1;
	while(HAL_GetTick() < 100)
	HAL_Delay(1);

	W25QFLASH_CS_UNSELECT(dev);(dev);
	HAL_Delay(100);

	uint32_t id;
	
	id = W25qxx_ReadID(dev);

	switch(id & 0x0000FFFF)
	{
		case 0x401A:	// 	w25q512
			dev->device_model = W25Q512;
			dev->BlockCount = 1024;
		break;

		case 0x4019:	// 	w25q256
			dev->device_model = W25Q256;
			dev->BlockCount = 512;
		break;

		case 0x4018:	// 	w25q128
			dev->device_model = W25Q128;
			dev->BlockCount = 256;
		break;

		case 0x4017:	//	w25q64
			dev->device_model = W25Q64;
			dev->BlockCount = 128;
		break;

		case 0x4016:	//	w25q32
			dev->device_model = W25Q32;
			dev->BlockCount = 64;
		break;

		case 0x4015:	//	w25q16
			dev->device_model = W25Q16;
			dev->BlockCount = 32;
		break;

		case 0x4014:	//	w25q80
			dev->device_model = W25Q80;
			dev->BlockCount = 16;
		break;

		case 0x4013:	//	w25q40
			dev->device_model = W25Q40;
			dev->BlockCount = 8;
		break;

		case 0x4012:	//	w25q20
			dev->device_model = W25Q20;
			dev->BlockCount = 4;
		break;

		case 0x4011:	//	w25q10
			dev->device_model = W25Q10;
			dev->BlockCount = 2;
		break;

		case 0x3017:	//	w25x64
			dev->device_model = W25Q64;
			dev->BlockCount = 128;
		break;

		case 0x3016:	//	w25x32
			dev->device_model = W25Q32;
			dev->BlockCount = 64;
		break;

		case 0x3015:	//	w25q16
			dev->device_model = W25Q16;
			dev->BlockCount = 32;
		break;
		case 0x3014:	//	w25x80
			dev->device_model = W25Q80;
			dev->BlockCount = 16;
		break;

		case 0x3013:	//	w25x40
			dev->device_model = W25Q40;
			dev->BlockCount = 8;
		break;

		case 0x3012:	//	w25x20
			dev->device_model = W25Q20;
			dev->BlockCount = 4;
		break;

		case 0x3011:	//	w25x10
			dev->device_model = W25Q10;
			dev->BlockCount = 2;
		break;

		default:

			dev->Lock = 0;
			return 0;
	}


	dev->PageSize = 256;
	dev->SectorSize = 0x1000;
	dev->SectorCount = dev->BlockCount * 16;
	dev->PageCount = (dev->SectorCount * dev->SectorSize) / dev->PageSize;
	dev->BlockSize = dev->SectorSize * 16;
	dev->CapacityInKiloByte = (dev->SectorCount * dev->SectorSize) / 1024;

	dev->Lock = 0;
	return 1;

}

void w25qxx_EraseChip(W25x_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	W25x_WriteEnable(dev);

	W25QFLASH_CS_SELECT(dev);
	W25qxx_Spi(W25_CHIP_ERASE, dev);
	W25QFLASH_CS_UNSELECT(dev);(dev);

	W25qxx_WaitForWriteEnd(dev);

	HAL_Delay(10);

	dev->Lock = 0;
}

void W25qxx_EraseSector(uint32_t SectorAddr, W25x_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	W25qxx_WaitForWriteEnd(dev);
	SectorAddr = SectorAddr * dev->SectorSize;

	W25x_WriteEnable(dev);

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_SECTOR_ERASE, dev);

	if(dev->device_model >= W25Q256)
		W25qxx_Spi((SectorAddr & 0xFF000000) >> 24, dev);

	W25qxx_Spi((SectorAddr & 0xFF0000) >> 16, dev);
	W25qxx_Spi((SectorAddr & 0xFF00) >> 8, dev);
	W25qxx_Spi(SectorAddr & 0xFF, dev);

	W25QFLASH_CS_UNSELECT(dev);

	W25qxx_WaitForWriteEnd(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

void W25qxx_EraseBlock(uint32_t BlockAddr, W25x_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	W25qxx_WaitForWriteEnd(dev);

	BlockAddr = BlockAddr * dev->SectorSize * 16;

	W25x_WriteEnable(dev);

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_BLOCK_ERASE, dev);

	if(dev->device_model>=W25Q256)
		W25qxx_Spi((BlockAddr & 0xFF000000) >> 24, dev);

	W25qxx_Spi((BlockAddr & 0xFF0000) >> 16, dev);
	W25qxx_Spi((BlockAddr & 0xFF00) >> 8, dev);
	W25qxx_Spi(BlockAddr & 0xFF, dev);

	W25QFLASH_CS_UNSELECT(dev);

	W25qxx_WaitForWriteEnd(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

uint32_t W25qxx_PageToSector(uint32_t PageAddress, W25x_device_t *dev)
{
	return((PageAddress * dev->PageSize) / dev->SectorSize);
}

uint32_t W25qxx_PageToBlock(uint32_t PageAddress, W25x_device_t *dev)
{
	return((PageAddress * dev->PageSize) / dev->BlockSize);
}


uint32_t W25qxx_SectorToBlock(uint32_t SectorAddress, W25x_device_t *dev)
{
	return((SectorAddress * dev->SectorSize) / dev->BlockSize);
}


uint32_t W25qxx_SectorToPage(uint32_t SectorAddress, W25x_device_t *dev)
{
	return(SectorAddress * dev->SectorSize) / dev->PageSize;
}


uint32_t W25qxx_BlockToPage(uint32_t BlockAddress, W25x_device_t *dev)
{
	return (BlockAddress * dev->BlockSize) / dev->PageSize;
}


uint8_t W25qxx_IsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte, W25x_device_t *dev)
{
	while(dev->Lock == 1)
	HAL_Delay(1);

	dev->Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t size = 0;

	size = dev->PageSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Page_Address * dev->PageSize);

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_FAST_READ, dev);

	if(dev->device_model >= W25Q256)
		W25qxx_Spi((WorkAddress & 0xFF000000) >> 24, dev);

	W25qxx_Spi((WorkAddress & 0xFF0000) >> 16, dev);
	W25qxx_Spi((WorkAddress & 0xFF00) >> 8, dev);
	W25qxx_Spi(WorkAddress & 0xFF, dev);

	W25qxx_Spi(0, dev);

	HAL_SPI_Receive(dev->Interface.spi_handle, pBuffer, size, 100);

	W25QFLASH_CS_UNSELECT(dev);

	for(uint16_t i = 0; i < size; i++)
	{
		if(pBuffer[i] != 0xFF)
		{
			dev->Lock = 0;
			return 0;
		}
	}

	dev->Lock = 0;
	return 1;
}

uint8_t W25qxx_IsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte, W25x_device_t *dev)
{
	while(dev->Lock == 1)
	HAL_Delay(1);

	dev->Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t s_buf = 256;
	uint16_t size = 0;

	size = dev->SectorSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Sector_Address * dev->SectorSize);


	uint16_t cikl = size / 256;
	uint16_t cikl2 = size % 256;
	uint16_t count_cikle = 0;

	if(size <= 256)
	{
		count_cikle = 1;
	}
	else if(cikl2 == 0)
	{
		count_cikle = cikl;
	}
	else
	{
		count_cikle = cikl + 1;
	}


	for(uint16_t i = 0; i < count_cikle; i++)
	{
		W25QFLASH_CS_SELECT(dev);
		W25qxx_Spi(W25_FAST_READ, dev);

		if(dev->device_model>=W25Q256)
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24, dev);

		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16, dev);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8, dev);
		W25qxx_Spi(WorkAddress & 0xFF, dev);

		W25qxx_Spi(0, dev);

		if(size < 256) s_buf = size;

		HAL_SPI_Receive(dev->Interface.spi_handle, pBuffer, s_buf, 100);

		W25QFLASH_CS_UNSELECT(dev);

		for(uint16_t i = 0; i < s_buf; i++)
		{
			if(pBuffer[i] != 0xFF)
			{
				dev->Lock = 0;
				return 0;
			}
		}

		size = size - 256;
		WorkAddress = WorkAddress + 256;
	}

	dev->Lock = 0;
	return 1;
}

uint8_t W25qxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, W25x_device_t *dev)
{
	while(dev->Lock == 1)
	HAL_Delay(1);

	dev->Lock = 1;

	uint8_t	pBuffer[256] = {0,};
	uint32_t WorkAddress = 0;
	uint16_t s_buf = 256;
	uint32_t size = 0;

	size = dev->BlockSize - OffsetInByte;
	WorkAddress = (OffsetInByte + Block_Address * dev->BlockSize);

	uint16_t cikl = size / 256;
	uint16_t cikl2 = size % 256;
	uint16_t count_cikle = 0;

	if(size <= 256)
	{
		count_cikle = 1;
	}
	else if(cikl2 == 0)
	{
		count_cikle = cikl;
	}
	else
	{
		count_cikle = cikl + 1;
	}


	for(uint16_t i = 0; i < count_cikle; i++)
	{
		W25QFLASH_CS_SELECT(dev);
		W25qxx_Spi(W25_FAST_READ, dev);

		if(dev->device_model>=W25Q256)
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24, dev);

		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16, dev);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8, dev);
		W25qxx_Spi(WorkAddress & 0xFF, dev);

		W25qxx_Spi(0, dev);

		if(size < 256) s_buf = size;

		HAL_SPI_Receive(dev->Interface.spi_handle, pBuffer, s_buf, 100);

		W25QFLASH_CS_UNSELECT(dev);

		for(uint16_t i = 0; i < s_buf; i++)
		{
			if(pBuffer[i] != 0xFF)
			{
				dev->Lock = 0;
				return 0;
			}
		}

		size = size - 256;
		WorkAddress = WorkAddress + 256;
	}

	dev->Lock = 0;
	return 1;
}

void W25qxx_WritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize, W25x_device_t *dev)
{
	while(dev->Lock == 1)
		HAL_Delay(1);

	dev->Lock = 1;

	if(((NumByteToWrite_up_to_PageSize + OffsetInByte) > dev->PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = dev->PageSize - OffsetInByte;

	if((OffsetInByte + NumByteToWrite_up_to_PageSize) > dev->PageSize)
		NumByteToWrite_up_to_PageSize = dev->PageSize - OffsetInByte;


	W25qxx_WaitForWriteEnd(dev);

	W25x_WriteEnable(dev);

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_PAGE_PROGRAMM, dev);

	Page_Address = (Page_Address * dev->PageSize) + OffsetInByte;

	if(dev->device_model >= W25Q256)
		W25qxx_Spi((Page_Address & 0xFF000000) >> 24, dev);

	W25qxx_Spi((Page_Address & 0xFF0000) >> 16, dev);
	W25qxx_Spi((Page_Address & 0xFF00) >> 8, dev);
	W25qxx_Spi(Page_Address & 0xFF, dev);

	HAL_SPI_Transmit(dev->Interface.spi_handle, pBuffer, NumByteToWrite_up_to_PageSize, 100);

	W25QFLASH_CS_UNSELECT(dev);

	W25qxx_WaitForWriteEnd(dev);

	HAL_Delay(1);
	dev->Lock = 0;
}

void W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize, W25x_device_t *dev)
{
	if((NumByteToWrite_up_to_SectorSize > dev->SectorSize) || (NumByteToWrite_up_to_SectorSize == 0))
		NumByteToWrite_up_to_SectorSize = dev->SectorSize;

	uint32_t StartPage;
	int32_t	BytesToWrite;
	uint32_t LocalOffset;

	if((OffsetInByte + NumByteToWrite_up_to_SectorSize) > dev->SectorSize)
		BytesToWrite = dev->SectorSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_SectorSize;	

	StartPage = W25qxx_SectorToPage(Sector_Address, dev) + (OffsetInByte / dev->PageSize);
	LocalOffset = OffsetInByte % dev->PageSize;

	do
	{		
		W25qxx_WritePage(pBuffer, StartPage, LocalOffset, BytesToWrite, dev);
		StartPage++;

		BytesToWrite -= dev->PageSize - LocalOffset;
		//pBuffer += dev->PageSize;
		pBuffer += dev->PageSize - LocalOffset;
		LocalOffset = 0;
	}
	while(BytesToWrite > 0);
}

void W25qxx_WriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize, W25x_device_t *dev)
{
	if((NumByteToWrite_up_to_BlockSize>dev->BlockSize)||(NumByteToWrite_up_to_BlockSize == 0))
		NumByteToWrite_up_to_BlockSize=dev->BlockSize;

	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;

	if((OffsetInByte+NumByteToWrite_up_to_BlockSize) > dev->BlockSize)
		BytesToWrite = dev->BlockSize - OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_BlockSize;	

	StartPage = W25qxx_BlockToPage(Block_Address, dev)+(OffsetInByte/dev->PageSize);

	LocalOffset = OffsetInByte%dev->PageSize;	

	do
	{		
		W25qxx_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite, dev);
		StartPage++;
		BytesToWrite -= dev->PageSize - LocalOffset;
		pBuffer += dev->PageSize - LocalOffset;
		LocalOffset = 0;
	}
	while(BytesToWrite > 0);
}

HAL_StatusTypeDef w25x_writemem(W25x_device_t *dev, uint8_t *buff, size_t len, size_t addr){
    if(len > dev->BlockCount * dev->BlockSize){
        return HAL_ERROR;
    }
    while(len > 0){
        W25qxx_WriteBlock(buff, addr/dev->BlockSize, addr%dev->BlockSize, len, dev);
        len-= dev->BlockSize;
        addr += dev->BlockSize;
        addr -= (addr%dev->BlockSize);
        buff += dev->BlockSize;
    }
    return HAL_OK;
}

HAL_StatusTypeDef w25x_readmem(W25x_device_t *dev, uint8_t *buff, size_t len, size_t addr){
    while(dev->Lock == 1)
	HAL_Delay(1);

	dev->Lock = 1;

	W25QFLASH_CS_SELECT(dev);

	W25qxx_Spi(W25_FAST_READ, dev);

	if(dev->device_model >= W25Q256)
		W25qxx_Spi((addr & 0xFF000000) >> 24, dev);

	W25qxx_Spi((addr & 0xFF0000) >> 16, dev);
	W25qxx_Spi((addr& 0xFF00) >> 8, dev);
	W25qxx_Spi(addr & 0xFF, dev);
	W25qxx_Spi(0, dev);

	HAL_StatusTypeDef status = HAL_SPI_Receive(dev->Interface.spi_handle, buff, len, 2000);

	W25QFLASH_CS_UNSELECT(dev);

	HAL_Delay(1);
	dev->Lock = 0;
    return status;
}