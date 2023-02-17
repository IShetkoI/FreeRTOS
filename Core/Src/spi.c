/**
   ******************************************************************************
   * @file    spi.c
   * @author  Ivan Shetska
   * @brief   This is the common part of the Spi initialization
   ******************************************************************************
   */

#include "spi.h"


SPI_HandleTypeDef hspi1; ///< Pointer to the descriptor of the selected SPI hardware device


HAL_StatusTypeDef initializeSpi (void)
{
    /* SPI1 parameter configuration*/
    hspi1.Instance               = SPI1;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 10;
    hspi1.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init (&hspi1) != HAL_OK)
        return HAL_ERROR;
    else
        return HAL_OK;
}


SPI_HandleTypeDef getPointerSpi (void)
{
    return hspi1;
}


/**
   ******************************************************************************
   * @brief      SPI MSP Initialization
   *             This function configures the hardware resources used in this
   *             example
   * @ingroup    spi
   * @param[in]  hspi - SPI handle pointer
   ******************************************************************************
   */

void HAL_SPI_MspInit (SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE ();

        __HAL_RCC_GPIOA_CLK_ENABLE ();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

        HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);
    }
}


/**
   ******************************************************************************
   * @brief      SPI MSP De-Initialization
   *             This function freeze the hardware resources used in this example
   * @ingroup    spi
   * @param[in]  hspi - SPI handle pointer
   ******************************************************************************
   */

void HAL_SPI_MspDeInit (SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE ();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit (GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    }
}
