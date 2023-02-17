/**
   ******************************************************************************
   * @file    bmp280.h
   * @author  Ivan Shetska
   * @brief   This file contains prototype functions for working with the
   *          BMP280 sensor
   ******************************************************************************
   */

#ifndef BMP280_H
#define BMP280_H

/* Define to prevent recursive inclusion --------------------------------------*/
#include "bmp280_defines.h"
#include "stdbool.h"
#include "stdio.h"

/**
   ******************************************************************************
   * @defgroup bmp BMP280
   ******************************************************************************
   * @{
   */


/// Contains the values of the last measurement
typedef struct
{
	int temperature; ///< Measured temperature
	int pressure;    ///< Measured pressure
} Measurement;


/// Contains values for compensation calculations
typedef struct
{
    uint16_t digT1;
    int16_t  digT2;
    int16_t  digT3;
    uint16_t digP1;
    int16_t  digP2;
    int16_t  digP3;
    int16_t  digP4;
    int16_t  digP5;
    int16_t  digP6;
    int16_t  digP7;
    int16_t  digP8;
    int16_t  digP9;
} CompensationParameters;


/// Contains the current sensor configuration
typedef struct
{
    Oversampling      pressureOversampling;    ///< Pressure oversampling
    Oversampling      temperatureOversampling; ///< Temperature oversampling
    PowerMode         powerMode;               ///< Power Mode
    FilterCoefficient filterCoefficient;       ///< Filter coefficient
    StandbyTime       standbyTime;             ///< Standby time
} Configuration;


/// Contains basic information about the sensor
typedef struct
{
    SPI_HandleTypeDef             handle;                 ///< SPI handle pointer
    uint8_t                       deviceID;               ///< Device ID
    Measurement            measurement;            ///< Structure of the values of the last measurement
    CompensationParameters compensationParameters; ///< Compensation parameter structure
    Configuration          configuration;          ///< Current sensor configuration structure
} BMP280;


/**
   ******************************************************************************
   * @brief      Sensor initialization function
   * @param[in]  device                  - Pointer to the descriptor of the
   *                                       selected SPI hardware device
   * @param[in]  handle                  - SPI handle pointer
   * @param[in]  deviceID                - Device ID
   * @param[in]  pressureOversampling    - Pressure oversampling
   * @param[in]  temperatureOversampling - Temperature oversampling
   * @param[in]  powerMode               - Sensor power mode
   * @param[in]  filterCoefficient       - Filter coefficient
   * @param[in]  standbyTime             - Standby time
   * @returns    Status of initialization
   ******************************************************************************
  */

StatusCodes initializeBmp280 (BMP280 *device,
                              SPI_HandleTypeDef *handle,
                              uint8_t deviceID,
                              Oversampling pressureOversampling,
                              Oversampling temperatureOversampling,
                              PowerMode powerMode,
                              FilterCoefficient filterCoefficient,
                              StandbyTime standbyTime);


/**
   ******************************************************************************
   * @brief      Function to read sensor ID and аuthentication
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   * @returns    Status of readout and serviceability check execution
   ******************************************************************************
  */

StatusCodes readPartID (BMP280 *device);


/**
   ******************************************************************************
   * @brief      BMP280 software reset function
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void softReset (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function to set pressure oversampling
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void setPressureOversampling (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function to set temperature oversampling
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void setTemperatureOversampling (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function to set power mode
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void setPowerMode (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function to set standby time
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void setStandbyTime (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function to set filter coefficients
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   ******************************************************************************
  */

void setFilterCoefficient (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function for reading compensation parameters from the sensor
   *             memory
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   * @returns    Data reading status
   ******************************************************************************
  */

StatusCodes readCompensationParameters (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Function for reading raw data from the sensor memory and
   *             compensating their values
   * @param[in]  device - Pointer to the descriptor of the selected SPI hardware
   *                      device
   * @returns    Data reading and conversion status
   ******************************************************************************
  */

StatusCodes measure (BMP280 *device);


/**
   ******************************************************************************
   * @brief      Raw temperature compensation function
   * @param[in]  device                   - Pointer to the descriptor of the
   *                                        selected SPI hardware device
   * @param[in]  uncompensatedTemperature - Uncompensated temperature
   * @returns    Compensated temperature
   ******************************************************************************
  */

int32_t compensateTemperature (BMP280 *device, int32_t uncompensatedTemperature);


/**
   ******************************************************************************
   * @brief      Raw pressure compensation function
   * @param[in]  device                - Pointer to the descriptor of the
   *                                     selected SPI hardware device
   * @param[in]  uncompensatedPressure - Uncompensated pressure
   * @returns    Compensated pressure
   ******************************************************************************
  */

uint32_t compensatePressure (BMP280 *device, int32_t uncompensatedPressure);


/**
   ******************************************************************************
   * @brief      SPI register reading function by the specified address
   * @param[in]  device  - Pointer to the descriptor of the selected SPI hardware
   *                       device
   * @param[in]  address - Reading address
   * @returns    The read value at the given address
   ******************************************************************************
  */

uint16_t readRegister (BMP280 *device, uint8_t address);


/**
   ******************************************************************************
   * @brief      Function for writing to registers via SPI at a given address
   * @param[in]  device  - Pointer to the descriptor of the selected SPI hardware
   *                       device
   * @param[in]  address - Reading address
   * @param[in]  value   - Value for the record
   ******************************************************************************
  */

void writeRegister (BMP280 *device, uint8_t address, uint8_t value);


/**
   ******************************************************************************
   * @brief      Function for reading a given amount of information via SPI at a
   *             given address
   * @param[in]  device  - Pointer to the descriptor of the selected SPI hardware
   *                       device
   * @param[in]  address - Reading address
   * @param[in]  value   - Value for the record
   * @param[in]  length  - Data size for reading
   ******************************************************************************
  */

void readMultiByteRegister (BMP280 *device, uint8_t address, uint8_t *values, uint8_t length);


/**
   ******************************************************************************
   * @brief      Basic SPI read/write function
   * @param[in]  handle    - SPI handle pointer
   * @param[in]  txMessage - Message for the transmit
   * @return     Message read
   ******************************************************************************
  */

uint8_t spiReadWrite (SPI_HandleTypeDef handle, uint8_t txMessage);


/**
   ******************************************************************************
   * @brief      Function to start pairing with the sensor
   * @param[in]  crystalSelectPin - Number of the pin on which the sensor is
   *             located
   ******************************************************************************
  */

void spiCsHigh (uint8_t crystalSelectPin);


/**
   ******************************************************************************
   * @brief      End of pairing function with the sensor
   * @param[in]  crystalSelectPin - Number of the pin on which the sensor is
   *             located
   ******************************************************************************
  */

void spiCsLow (uint8_t crystalSelectPin);


#endif //BMP280_H
