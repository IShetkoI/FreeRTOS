/**
   ******************************************************************************
   * @file    bmp280_compensation.c
   * @author  Ivan Shetska
   * @brief   This is a common part of the compensation functions for the sensor
   ******************************************************************************
   */

#include <stdint.h>
#include "bmp280.h"


int32_t compensateTemperature (BMP280 *device, int32_t uncompensatedTemperature)
{
    /* Example compensation taken from bst-bmp280-ds001.pdf chapter 8 paragraph 8.1 */

    int32_t var1, var2, temperature;

    var1 = ((((uncompensatedTemperature / 8) - ((int32_t) device->compensationParameters.digT1 << 1))) *
            ((int32_t) device->compensationParameters.digT2)) / 2048;

    var2 = (((((uncompensatedTemperature / 16) - ((int32_t) device->compensationParameters.digT1)) *
              ((uncompensatedTemperature / 16) - ((int32_t) device->compensationParameters.digT1))) / 4096) *
            ((int32_t) device->compensationParameters.digT3)) / 16384;

    temperature = var1 + var2;

    return (temperature * 5 + 128) / 256;
}


uint32_t compensatePressure (BMP280 *device, int32_t uncompensatedPressure)
{
    /* Example compensation taken from bst-bmp280-ds001.pdf chapter 8 paragraph 8.1 */

    int64_t var1, var2, pressure;

    var1 = ((int64_t) (device->measurement.temperature)) - 128000;

    var2 = var1 * var1 * (int64_t) device->compensationParameters.digP6;
    var2 = var2 + ((var1 * (int64_t) device->compensationParameters.digP5) * 131072);
    var2 = var2 + (((int64_t) device->compensationParameters.digP4) * 34359738368);

    var1 = ((var1 * var1 * (int64_t) device->compensationParameters.digP3) / 256) +
           ((var1 * (int64_t) device->compensationParameters.digP2) * 4096);
    var1 = ((INT64_C (0x800000000000) + var1) * ((int64_t) device->compensationParameters.digP1)) / 8589934592;

    if (var1 == 0)
        return 0;

    pressure = 1048576 - uncompensatedPressure;
    pressure = (((((pressure * 2147483648U)) - var2) * 3125) / var1);

    var1 = (((int64_t) device->compensationParameters.digP9) * (pressure / 8192) * (pressure / 8192)) / 33554432;

    var2 = (((int64_t) device->compensationParameters.digP8) * pressure) / 524288;

    pressure = ((pressure + var1 + var2) / 256) + (((int64_t) device->compensationParameters.digP7) * 16);

    return (uint32_t) pressure;
}
