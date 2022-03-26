#include "eeprom.h"
#include "switch.h"

uint8 switch_submodeEEP = 0;
static uint8 EEPROM_Write_Counter = 0;
static uint32 EEPROM_Write_Address = EEPROM_PAGE127_START;

void EEPROM_Write(void)
{
    uint16 data[8] = {0};
    data[EEPROM_Write_Counter] = switch_submode;
    /* Unlock to control */
    HAL_FLASH_Unlock();

    /* Writing data to flash memory */
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, EEPROM_Write_Address, data[EEPROM_Write_Counter]);
    EEPROM_Write_Address += 0x02;

    /* Lock flash control register */
    HAL_FLASH_Lock();
    EEPROM_Write_Counter++;
    if(EEPROM_Write_Counter>7)
    {
        EEPROM_Write_Counter = 0;
        EEPROM_Write_Address = EEPROM_PAGE127_START;
    }

    return HAL_OK;
}
void EEPROM_Read(void)
{
    uint16 data[8] = {0,};
    for(uint8 i = 0; i<8;i++)
    {
       data[i] = *(uint32*)(EEPROM_PAGE127_START+(i*2));
    }
    
    switch_submodeEEP = data[0];
}

void EEPROM_Erase(void)
{
    uint32_t SectorError = 0;
    
    /* Unlock to control */
    HAL_FLASH_Unlock();
    
    /* Calculate sector index */
    uint32_t UserAddress = EEPROM_PAGE127_START;
    uint32_t NbOfSectors = 1;
    
    /* Erase sectors */
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = UserAddress;
    EraseInitStruct.NbPages = NbOfSectors;
    
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    { 
        uint32_t errorcode = HAL_FLASH_GetError();            
        return HAL_ERROR;
    }
    
    /* Lock flash control register */
    HAL_FLASH_Lock();
    
    return HAL_OK;   
}
