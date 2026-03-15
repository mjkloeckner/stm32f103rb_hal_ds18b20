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

#define G_TASK_SEN_CNT_INIT     0ul
#define G_TASK_SEN_TICK_CNT_INI 0ul

#define DEL_BTN_XX_MIN          0ul
#define DEL_BTN_XX_MED          25ul
#define DEL_BTN_XX_MAX          50ul

const task_sensor_cfg_t task_sensor_cfg_list[] = {
};

#define SENSOR_CFG_QTY (sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {0};

#define SENSOR_DTA_QTY (sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

void task_sensor_statechart(void);

uint32_t g_task_sensor_cnt;

// this variable is incremented by 1 on every ms by HAL_SysTick
volatile uint32_t g_task_sensor_tick_cnt;

void task_sensor_init(void *parameters)
{
    // task_sensor_dta_t *p_task_sensor_dta;
    // task_sensor_st_t state;
    // task_sensor_ev_t event;

    LOGGER_INFO("Initializing `task_sensor`...");

    g_task_sensor_cnt = G_TASK_SEN_CNT_INIT;

    for (uint32_t index = 0; SENSOR_DTA_QTY > index; index++)
    {
        /* Init every associated sensor variables */
        // p_task_sensor_dta = &task_sensor_dta_list[index];

        // ...
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
        g_task_sensor_cnt++;

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

        // unused
        (void) p_task_sensor_cfg;
        (void) p_task_sensor_dta;
    }
}
