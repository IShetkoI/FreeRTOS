/**
   ******************************************************************************
   * @file    bmp280.h
   * @author  Ivan Shetska
   * @brief   This file contains all defines for the BMP280 sensor
   ******************************************************************************
   */

#ifndef BMP280_DEFINES_H_
#define BMP280_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


#define BMP280_CHIP_ID        0x58 ///< Correct BMP280 sensor ID

#define BMP280_SPI_MASK_WRITE 0x7F ///< SPI write mask

#define BMP280_REG_ID        0xD0 ///< Sensor ID location address
#define BMP280_REG_RESET     0xE0 ///< Sensor reset register address
#define BMP280_REG_STATUS    0xF3 ///< Sensor operation status register address
#define BMP280_REG_CTRL_MEAS 0xF4 ///< Sensor measurement configuration register address
#define BMP280_REG_CONFIG    0xF5 ///< Sensor options configuration register address
#define BMP280_REG_PRESS     0xF7 ///< Register address with raw pressure values
#define BMP280_REG_TEMP      0xFA ///< Register address with raw temperature values
#define BMP280_REG_CALIB     0x88 ///< Register address with calibration parameters

#define BMP280_PRESSURE_OVERSAMPLING_MASK    0xE3 ///< Pressure oversampling mask
#define BMP280_TEMPERATURE_OVERSAMPLING_MASK 0x1F ///< Temperature oversampling mask
#define BMP280_POWER_MODE_MASK               0xFC ///< Sensor operating mode mask
#define BMP280_STANDBY_TIME_MASK             0x1F ///< Standby time mask
#define BMP280_FILTER_COEFFICIENT_MASK       0xE3 ///< Mask of filter coefficients

#define BMP280_REG_DATA    0xF7 ///< Register address with raw data
#define BMP280_RESET_VALUE 0xB6 ///< Value for software sensor reset

#define SPI_RX_MAX_LENGTH  255 ///< Maximum size of the received message
#define SPI_BUF_MAX_LENGTH 24  ///< Maximum buffer size for correction factors
#define SPI_DATA_MAX_LENGTH 6  ///< Maximum data buffer size

#define BMP280_BUF_SHIFT    8  ///< Shift value for the buffer
#define BMP280_DATA_SHIFT_1 12 ///< Shift value of the first number of the data buffer
#define BMP280_DATA_SHIFT_2 4  ///< Shift value of the next numbers of the data buffer

#define BMP280_COMPENSATION_TEMPERATURE 100.0 ///< Last temperature divider
#define BMP280_COMPENSATION_PRESSURE    256.0 ///< Last pressure divider

#define SPI_DELAY 500 ///< Delay after reset


/// Possible oversampling options
typedef enum
{
    oversampling_skipped = 0b000,
    oversampling_x1      = 0b001,
    oversampling_x2      = 0b010,
    oversampling_x4      = 0b011,
    oversampling_x8      = 0b100,
    oversampling_x16     = 0b101
} Oversampling;


/// Possible operating modes of the sensor
typedef enum
{
    mode_sleep = 0b00, mode_forced = 0b01, mode_normal = 0b11
} PowerMode;


/// Possible standby time options
typedef enum
{
    standby_time_500us   = 0b000,
    standby_time_62500us = 0b001,
    standby_time_125ms   = 0b010,
    standby_time_250ms   = 0b011,
    standby_time_500ms   = 0b100,
    standby_time_1000ms  = 0b101,
    standby_time_2000ms  = 0b110,
    standby_time_4000ms  = 0b111
} StandbyTime;


/// Possible variants of filtering coefficients
typedef enum
{
    filter_off = 0b000, filter_coeff_2 = 0b001, filter_coeff_4 = 0b010, filter_coeff_8 = 0b011, filter_coeff_16 = 0b100
} FilterCoefficient;


/// Possible status and error codes of operations
typedef enum
{
    BMP280_OK,

    BMP280_MEASURE_NOT_READY,
    BMP280_MEASURE_IS_READY,

    BMP280_ERROR_INVALID_SPI_INSTANCE,
    BMP280_ERROR_INVALID_CRYSTAL_SELECT_PIN,
    BMP280_ERROR_INVALID_POWER_MODE,
    BMP280_ERROR_INVALID_DEVICE_ID,
    BMP280_ERROR_READ_COMPENSATION_PARAMETERS,
    BMP280_ERROR_READ_WRITE,
} StatusCodes;


#ifdef __cplusplus
}
#endif

#endif /* BMP280_DEFINES_H_ */
