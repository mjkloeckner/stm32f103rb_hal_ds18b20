/*
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

#include "ds18b20.h"
#include "logger.h"

uint8_t b_DS18B20_Received_data;
uint8_t g_DS18B20_Received_data_buffer[8];
// float g_DS18B20_temp;
// extern UART_HandleTypeDef huart1;

void UART1_SetBaud(uint32_t baud)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = baud;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    };
}

uint8_t DS18B20_Reset(void)
{
    uint8_t data = 0xF0;
    UART1_SetBaud(9600);
    HAL_UART_Transmit(&huart1, &data, 1, 100);

    if ((HAL_UART_Receive(&huart1, &data, 1, 1000) != HAL_OK) || (data == 0xF0))
    {
        // initialization failed
        LOGGER_ERROR("Couldn't read temperature sensor");
        return 1;
    }

    UART1_SetBaud(115200);
    return 0;
}

void DS18B20_SetResolution(uint8_t resolution)
{
    uint8_t config;

    switch(resolution)
    {
        case 9:  config = 0x1F; break;
        case 10: config = 0x3F; break;
        case 11: config = 0x5F; break;
        case 12: config = 0x7F; break;
        default: config = 0x7F;
    }

    DS18B20_Reset();
    DS18B20_Write(0xCC); // send 'Skip ROM' command
    DS18B20_Write(0x4E); // send 'write scratchpad' command

    DS18B20_Write(0x00); // TH
    DS18B20_Write(0x00); // TL
    DS18B20_Write(config);

    DS18B20_Reset();

    DS18B20_Write(0xCC);   // Skip ROM
    DS18B20_Write(0x48);   // Write scratchpad to EEPROM
}

void DS18B20_Init(void)
{
    if (DS18B20_Reset() == 0)
    {
        LOGGER_INFO("Temperature sensor initialized successfully");
    }
    DS18B20_SetResolution(10);
}

void DS18B20_Write(uint8_t data)
{
    uint8_t buffer[8];

    for (uint8_t i = 0; i < 8; ++i)
    {
        if (data & (1 << i))
        {
            buffer[i] = 0xFF;
        }
        else
        {
            buffer[i] = 0x00;
        }
    }

    HAL_UART_Transmit(&huart1, buffer, 8, 100);
}

uint8_t DS18B20_Read(void)
{
    uint8_t buffer[8];
    uint8_t received_value = 0;

    for (uint8_t i = 0; i < 8; ++i)
    {
        buffer[i] = 0xFF;
    }

    HAL_UART_Transmit_DMA(&huart1, buffer, 8);
    HAL_UART_Receive_DMA(&huart1, g_DS18B20_Received_data_buffer, 8);

    // while(b_DS18B20_Received_data == false)
    //     ;

    // b_DS18B20_Received_data = false;

    for (uint8_t i = 0; i < 8; ++i)
    {
        if (g_DS18B20_Received_data_buffer[i] == 0xFF)
        {
            received_value |= (1 << i);
        }
    }

    return received_value;
}

void DS18B20_Read_temp(void)
{
    DS18B20_Reset();
    DS18B20_Write(0xCC); // send 'Skip ROM' command
    DS18B20_Write(0x44); // send 'Convert T' command

    DS18B20_Reset();
    DS18B20_Write(0xCC); // send 'Skip ROM' command
    DS18B20_Write(0xBE); // send 'read scratchpad' command

    // uint8_t Temp_LSB = DS18B20_Read();
    // uint8_t Temp_MSB = DS18B20_Read();

    // uint16_t temp = (Temp_MSB << 8) | Temp_LSB;
    // g_DS18B20_temp = (float) temp / 16.0;
}
