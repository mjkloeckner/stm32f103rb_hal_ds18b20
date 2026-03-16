/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#include "main.h"

#include "logger.h"
#include "dwt.h"

#include "board.h"
#include "app.h"
#include "task_sensor_attribute.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "ds18b20.h"

#define G_TASK_SEN_TICK_CNT_INI   0ul
#define G_TASK_SEN_TEMP_PERIOD_MS 1000ul
#define G_DS18B20_CONV_TIME_MS    200ul // ~188ms for 10 bit resolution
#define G_DS18B20_DMA_TIMEOUT_MS  5000ul

const task_sensor_cfg_t task_sensor_cfg_list[] = {
    {ID_DS18B20}
};

#define SENSOR_CFG_QTY (sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {
    {G_TASK_SEN_TEMP_PERIOD_MS, ST_SEN_READY, EV_DS18B20_CONV_DONE}
};

#define SENSOR_DTA_QTY (sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

void task_sensor_statechart(void);

// external variables
volatile extern bool b_DS18B20_Received_data;
extern uint8_t g_DS18B20_Received_data_buffer[8];

// local global variables
volatile float g_DS18B20_temp;
uint32_t g_task_sensor_tick;
uint8_t Temp_LSB, Temp_MSB;
static bool b_DS18B20_Received_data_LSB;
static const uint8_t buffer[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    b_DS18B20_Received_data = true;
}

// this variable is incremented by 1 on every ms by HAL_SysTick
volatile uint32_t g_task_sensor_tick_cnt;

void task_sensor_init(void *parameters)
{
    const task_sensor_cfg_t *p_task_sensor_cfg;
    // task_sensor_dta_t *p_task_sensor_dta;

    LOGGER_INFO("Initializing `task_sensor`...");

    g_task_sensor_tick = G_TASK_SEN_TEMP_PERIOD_MS;

    for (uint32_t index = 0; SENSOR_DTA_QTY > index; index++)
    {
        p_task_sensor_cfg = &task_sensor_cfg_list[index];
        // p_task_sensor_dta = &task_sensor_dta_list[index];

        switch (p_task_sensor_cfg->identifier) {
            case ID_DS18B20:
                DS18B20_Init();
            break;

            default: break;
        }

    }

    LOGGER_INFO("Done initializing `task_sensor`");
}

void task_sensor_update(void *parameters)
{
    bool b_time_update_required = false;

    __asm("CPSID i");
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
    {
        g_task_sensor_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    while (b_time_update_required)
    {
        task_sensor_statechart();

        __asm("CPSID i");
        if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
        {
            g_task_sensor_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");
    }
}

void task_sensor_statechart(void)
{
    uint32_t index;
    const task_sensor_cfg_t *p_task_sensor_cfg;
    task_sensor_dta_t       *p_task_sensor_dta;

    for (index = 0; SENSOR_DTA_QTY > index; index++)
    {
        p_task_sensor_cfg = &task_sensor_cfg_list[index];
        p_task_sensor_dta = &task_sensor_dta_list[index];

        switch (p_task_sensor_cfg->identifier)
        {
            case ID_DS18B20:
                switch (p_task_sensor_dta->state)
                {
                    case ST_SEN_READY:
                        if (0 == g_task_sensor_tick)
                        {
                            // Send start temp conversion command
                            // this needs 96ms to 750ms depending on resolution
                            DS18B20_Reset();
                            DS18B20_Write(0xCC); // send 'Skip ROM' command
                            DS18B20_Write(0x44); // send 'Convert T' command

                            p_task_sensor_dta->state = ST_SEN_BUSY_WAITING_CONV;
                            g_task_sensor_tick = G_DS18B20_CONV_TIME_MS;
                        }
                        else
                        {
                            g_task_sensor_tick--;
                        }
                    break;

                    case ST_SEN_BUSY_WAITING_CONV:
                        if (0 == g_task_sensor_tick)
                        {
                            DS18B20_Reset();
                            DS18B20_Write(0xCC); // send 'Skip ROM' command
                            DS18B20_Write(0xBE); // send 'read scratchpad' command

                            if (HAL_OK == HAL_UART_Transmit_DMA(&huart1, buffer, 8))
                            {
                                if (HAL_OK == HAL_UART_Receive_DMA(&huart1, g_DS18B20_Received_data_buffer, 8))
                                {
                                    b_DS18B20_Received_data_LSB = true;
                                    p_task_sensor_dta->state = ST_SEN_BUSY_WAITING_DMA;
                                    g_task_sensor_tick = G_DS18B20_DMA_TIMEOUT_MS;
                                }
                            }
                        }
                        else
                        {
                            g_task_sensor_tick--;
                        }

                    break;

                    case ST_SEN_BUSY_WAITING_DMA:

                        if (0 == g_task_sensor_tick)
                        {
                            // DMA Timeout, reset conversion
                            p_task_sensor_dta->state = ST_SEN_READY;
                            g_task_sensor_tick = G_TASK_SEN_TEMP_PERIOD_MS;
                            break;
                        }
                        else
                        {
                            g_task_sensor_tick--;
                        }

                        if (true == b_DS18B20_Received_data)
                        {
                            b_DS18B20_Received_data = false;

                            uint8_t received_value = 0;
                            for (uint8_t i = 0; i < 8; ++i)
                            {
                                if (g_DS18B20_Received_data_buffer[i] == 0xFF)
                                {
                                    received_value |= (1 << i);
                                }
                            }

                            if (true == b_DS18B20_Received_data_LSB)
                            {
                                if (HAL_OK == HAL_UART_Transmit_DMA(&huart1, buffer, 8))
                                {
                                    if (HAL_OK == HAL_UART_Receive_DMA(&huart1, g_DS18B20_Received_data_buffer, 8))
                                    {
                                        b_DS18B20_Received_data_LSB = false;
                                        Temp_LSB = received_value;
                                        g_task_sensor_tick = G_DS18B20_DMA_TIMEOUT_MS;
                                    }
                                }
                            }
                            else
                            {
                                Temp_MSB = received_value;
                                uint16_t temp = ((Temp_MSB << 8) | Temp_LSB);
                                g_DS18B20_temp = (float) temp / 16.0;
                                p_task_sensor_dta->state = ST_SEN_READY;
                                g_task_sensor_tick = G_TASK_SEN_TEMP_PERIOD_MS;
                            }
                        }

                    break;

                    default: break;
                }
            break;

            default: break;
        }
    }
}
