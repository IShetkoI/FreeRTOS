/**
   ******************************************************************************
   * @file    bmp280_hardware.c
   * @author  Ivan Shetska
   * @brief   This is a common part of the read\write functions for the sensor
   ******************************************************************************
   */

#include "bmp280.h"


uint16_t readRegister (BMP280 *device, uint8_t address)
{
    spiCsLow (device->deviceID);
    spiReadWrite (device->handle, address);
    uint16_t value = spiReadWrite (device->handle, 0);
    spiCsHigh (device->deviceID);
    return value;
}


void writeRegister (BMP280 *device, uint8_t address, uint8_t value)
{
    spiCsLow (device->deviceID);

    spiReadWrite (device->handle, address & BMP280_SPI_MASK_WRITE);
    spiReadWrite (device->handle, value);

    spiCsHigh (device->deviceID);
}


void readMultiByteRegister (BMP280 *device, uint8_t address, uint8_t *values, uint8_t length)
{
    spiCsLow (device->deviceID);
    spiReadWrite (device->handle, address);

    while (length--)
    {
        *values++ = spiReadWrite (device->handle, 0);
    }

    spiCsHigh (device->deviceID);
}


uint8_t spiReadWrite (SPI_HandleTypeDef handle, uint8_t txMessage)
{
    uint8_t rxMessage = SPI_RX_MAX_LENGTH;

    HAL_SPI_TransmitReceive (&handle, &txMessage, &rxMessage, 1, HAL_MAX_DELAY);

    return rxMessage;
}


void spiCsHigh (uint8_t crystalSelectPin)
{
    HAL_GPIO_WritePin (CRYSTAL_SELECT_GPIO_Port, crystalSelectPin, GPIO_PIN_SET);
}


void spiCsLow (uint8_t crystalSelectPin)
{
    HAL_GPIO_WritePin (CRYSTAL_SELECT_GPIO_Port, crystalSelectPin, GPIO_PIN_RESET);
}


void delayMs (uint32_t milliseconds)
{
    HAL_Delay (milliseconds);
}
