/**
   ******************************************************************************
   * @file    bmp280.c
   * @author  Ivan Shetska
   * @brief   This is the common part of the BMP280 sensor
   ******************************************************************************
   */

#include "bmp280.h"
#include "spi.h"
#include "freertos.h"
#include <string.h>


StatusCodes initializeBmp280 (BMP280 *device,
                              SPI_HandleTypeDef *handle,
                              uint8_t deviceID,
                              Oversampling pressureOversampling,
                              Oversampling temperatureOversampling,
                              PowerMode powerMode,
                              FilterCoefficient filterCoefficient,
                              StandbyTime standbyTime)
{
    /* Status storage */
    StatusCodes status = BMP280_OK;

    /* If no data exchange interface has been set */
    if (handle->Instance == 0)
    {
        /* Error handling */
        status = BMP280_ERROR_INVALID_SPI_INSTANCE;
        errorHandler ();
    }
    else
    {
        device->handle = *handle;
    }


    /* If no sensor pin has been set */
    if (deviceID == 0)
    {
        status = BMP280_ERROR_INVALID_CRYSTAL_SELECT_PIN;
        errorHandler ();
    }
    else
    {
        device->deviceID = deviceID;
    }

    device->configuration.pressureOversampling    = pressureOversampling;
    device->configuration.temperatureOversampling = temperatureOversampling;


    /* If no power mode has been set */
    if (powerMode == 0)
    {
        status = BMP280_ERROR_INVALID_POWER_MODE;
        errorHandler ();
    }
    else
    {
        device->configuration.powerMode = powerMode;
    }

    device->configuration.filterCoefficient = filterCoefficient;
    device->configuration.standbyTime       = standbyTime;


    /* Sensor ID check */
    status = readPartID (device);

    if (status != BMP280_OK)
        errorHandler ();


    /* Reset sensor */
    softReset (device);

    HAL_Delay (SPI_DELAY);


    /* Setting the sensor configuration */
    setPressureOversampling (device);
    setTemperatureOversampling (device);

    setPowerMode (device);
    setFilterCoefficient (device);
    setStandbyTime (device);


    /* Reading parameters to compensate for measured sensor values */
    status = readCompensationParameters (device);

    return status;
}

StatusCodes readPartID (BMP280 *device)
{
    StatusCodes status = BMP280_OK;

    uint8_t chipID = readRegister (device, BMP280_REG_ID);

    if (chipID != BMP280_CHIP_ID)
    {
        status = BMP280_ERROR_INVALID_DEVICE_ID;
        errorHandler ();
    }

    return status;
}

void softReset (BMP280 *device)
{
    writeRegister (device, BMP280_REG_RESET, BMP280_RESET_VALUE);
}

void setPressureOversampling (BMP280 *device)
{
    /* Reading the installed configuration */
    uint8_t config = readRegister (device, BMP280_REG_CTRL_MEAS);

    /* Installing the new configuration */
    config = (config & BMP280_PRESSURE_OVERSAMPLING_MASK) | (device->configuration.pressureOversampling << 2);
    writeRegister (device, BMP280_REG_CTRL_MEAS, config);
}

void setTemperatureOversampling (BMP280 *device)
{
    uint8_t config = readRegister (device, BMP280_REG_CTRL_MEAS);
    config = (config & BMP280_TEMPERATURE_OVERSAMPLING_MASK) | (device->configuration.temperatureOversampling << 5);
    writeRegister (device, BMP280_REG_CTRL_MEAS, config);
}


void setPowerMode (BMP280 *device)
{
    uint8_t config = readRegister (device, BMP280_REG_CTRL_MEAS);
    config = (config & BMP280_POWER_MODE_MASK) | device->configuration.powerMode;
    writeRegister (device, BMP280_REG_CTRL_MEAS, config);
}

void setStandbyTime (BMP280 *device)
{
    uint8_t config = readRegister (device, BMP280_REG_CONFIG);
    config = (config & BMP280_STANDBY_TIME_MASK) | (device->configuration.standbyTime << 5);
    writeRegister (device, BMP280_REG_CONFIG, config);
}

void setFilterCoefficient (BMP280 *device)
{
    uint8_t config = readRegister (device, BMP280_REG_CONFIG);
    config = (config & BMP280_FILTER_COEFFICIENT_MASK) | (device->configuration.filterCoefficient << 2);
    writeRegister (device, BMP280_REG_CONFIG, config);
}

StatusCodes readCompensationParameters (BMP280 *device)
{
    StatusCodes status = BMP280_OK;

    uint8_t buf[SPI_BUF_MAX_LENGTH] = {0,};

    readMultiByteRegister (device, BMP280_REG_CALIB, buf, SPI_BUF_MAX_LENGTH);

    /* If the values have not been read */
    if (buf[1] == 0)
    {
        status = BMP280_ERROR_READ_COMPENSATION_PARAMETERS;
        errorHandler ();
    }

    /* Memorize compensation parameters */
    device->compensationParameters.digT1 = ((buf[1] << BMP280_BUF_SHIFT) | buf[0]);
    device->compensationParameters.digT2 = ((buf[3] << BMP280_BUF_SHIFT) | buf[2]);
    device->compensationParameters.digT3 = ((buf[5] << BMP280_BUF_SHIFT) | buf[4]);
    device->compensationParameters.digP1 = ((buf[7] << BMP280_BUF_SHIFT) | buf[6]);
    device->compensationParameters.digP2 = ((buf[9] << BMP280_BUF_SHIFT) | buf[8]);
    device->compensationParameters.digP3 = ((buf[11] << BMP280_BUF_SHIFT) | buf[10]);
    device->compensationParameters.digP4 = ((buf[13] << BMP280_BUF_SHIFT) | buf[12]);
    device->compensationParameters.digP5 = ((buf[15] << BMP280_BUF_SHIFT) | buf[14]);
    device->compensationParameters.digP6 = ((buf[17] << BMP280_BUF_SHIFT) | buf[16]);
    device->compensationParameters.digP7 = ((buf[19] << BMP280_BUF_SHIFT) | buf[18]);
    device->compensationParameters.digP8 = ((buf[21] << BMP280_BUF_SHIFT) | buf[20]);
    device->compensationParameters.digP9 = ((buf[23] << BMP280_BUF_SHIFT) | buf[22]);

    return status;
}


StatusCodes measure (BMP280 *device)
{
    StatusCodes status = BMP280_OK;

    uint8_t data[SPI_DATA_MAX_LENGTH];

    /* Read the raw measurements from the sensor memory */
    readMultiByteRegister (device, BMP280_REG_DATA, data, SPI_DATA_MAX_LENGTH);

    /* Convert to normal form */
    int32_t rawPressure    =
                    data[0] << BMP280_DATA_SHIFT_1 | data[1] << BMP280_DATA_SHIFT_2 | data[2] >> BMP280_DATA_SHIFT_2;
    int32_t rawTemperature =
                    data[3] << BMP280_DATA_SHIFT_1 | data[4] << BMP280_DATA_SHIFT_2 | data[5] >> BMP280_DATA_SHIFT_2;

    /* Compensate values */
    device->measurement.temperature =
            (float) compensateTemperature (device, rawTemperature) / BMP280_COMPENSATION_TEMPERATURE;
    device->measurement.pressure = (float) compensatePressure (device, rawPressure) / BMP280_COMPENSATION_PRESSURE;

    /* Checking for the readiness of new measurements */
    bool isTemperatureNull = device->measurement.temperature == 0;
    bool isPressureNull    = device->measurement.pressure == 0;

    if (isTemperatureNull && isPressureNull)
        status = BMP280_MEASURE_NOT_READY;
    else
        status = BMP280_MEASURE_IS_READY;

    return status;
}


void startTaskBMP280 (void *argument)
{
    queueUSART_t      messageUSART;
    BMP280            bmp280;
    char              temp[STRING_SIZE] = "";
    SPI_HandleTypeDef hspi1             = getPointerSpi ();
    osMutexId_t mutexErrorHandle = getMutexErrorHandle();
    osSemaphoreId_t semaphoreButtonHandle = getSemaphoreButtonHandle();

    osMessageQueueId_t queueUSARTHandle = getQueueUsartHandle();

    if (initializeBmp280 (&bmp280, &hspi1, CRYSTAL_SELECT_Pin, oversampling_x16, oversampling_x2, mode_normal,
                          filter_coeff_16, standby_time_500us) != BMP280_OK)
    {
        osMutexAcquire (mutexErrorHandle, osWaitForever);
    }

    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            if (osSemaphoreGetCount (semaphoreButtonHandle) == 0)
            {
                measure (&bmp280);
                strcpy (messageUSART.Buf, "Temperature - ");
                sprintf (temp, "%d", bmp280.measurement.temperature);
                strcat (messageUSART.Buf, temp);

                strcat (messageUSART.Buf, ";   Pressure - ");
                sprintf (temp, "%d", bmp280.measurement.pressure);
                strcat (messageUSART.Buf, temp);

                osMessageQueuePut (queueUSARTHandle, &messageUSART, 0, osWaitForever);

                HAL_GPIO_TogglePin (LED_BLUE_GPIO_Port, LED_BLUE_Pin);
                osDelay (FLASHING_PERIOD);
            }
        }

        osDelay (MINIMUM_DELAY);
    }
}
