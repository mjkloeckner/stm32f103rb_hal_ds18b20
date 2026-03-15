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
#define G_TASK_SEN_TEMP_PERIOD_MS 2000ul

const task_sensor_cfg_t task_sensor_cfg_list[] = {
    {ID_DS18B20, 0, EV_DS18B20_CONV_DONE}
};

#define SENSOR_CFG_QTY (sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {
    {G_TASK_SEN_TICK_CNT_INI, ST_SEN_READY}
};

#define SENSOR_DTA_QTY (sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

void task_sensor_statechart(void);

uint32_t g_task_sensor_tick;

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
    task_sensor_dta_t *p_task_sensor_dta;

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
                            DS18B20_Read_temp();
                            p_task_sensor_dta->event = EV_DS18B20_CONV_DONE;
                            p_task_sensor_dta->state = ST_SEN_BUSY;
                        }
                        else
                        {
                            g_task_sensor_tick--;
                        }
                    break;

                    case ST_SEN_BUSY:
                        if (EV_DS18B20_CONV_DONE == p_task_sensor_dta->event)
                        {
                            p_task_sensor_dta->state = ST_SEN_READY;
                            g_task_sensor_tick = G_TASK_SEN_TEMP_PERIOD_MS;
                        }
                    break;

                    default: break;
                }
            break;

            default: break;
        }
    }
}
