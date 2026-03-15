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
#include "task_system.h"
#include "task_actuator.h"
#include "task_sensor.h"

#define G_APP_CNT_INI       0ul
#define G_APP_TICK_CNT_INI  0ul
#define G_APP_WCET_INI      0ul

#define TASK_X_WCET_INI     0ul
#define TASK_X_DELAY_MIN    0ul

typedef struct {
    void (*task_init)(void *);
    void (*task_update)(void *);
    void *parameters;
} task_cfg_t;

typedef struct {
    uint32_t WCET; // Task worst-case execution time (microseconds)
} task_dta_t;

const task_cfg_t task_cfg_list[] = {
    {task_sensor_init,   task_sensor_update,   NULL},
    {task_system_init,   task_system_update,   NULL},
    {task_actuator_init, task_actuator_update, NULL}
};

#define TASK_QTY (sizeof(task_cfg_list)/sizeof(task_cfg_t))

uint32_t g_app_cnt;
uint32_t g_app_runtime_us;
uint32_t g_app_WCET_us; // Worst case execution time historically

volatile uint32_t g_app_tick_cnt;

task_dta_t task_dta_list[TASK_QTY];

void app_init(void)
{
    LOGGER_INFO("Initializing `app`...");

    g_app_WCET_us = G_APP_WCET_INI;

    cycle_counter_init();

    for (uint32_t index = 0; TASK_QTY > index; index++)
    {
        (*task_cfg_list[index].task_init)(task_cfg_list[index].parameters);
        task_dta_list[index].WCET = TASK_X_WCET_INI;
    }

    __asm("CPSID i");
    g_app_tick_cnt = G_APP_TICK_CNT_INI;
    g_task_sensor_tick_cnt = G_APP_TICK_CNT_INI;
    g_task_system_tick_cnt = G_APP_TICK_CNT_INI;
    g_task_actuator_tick_cnt = G_APP_TICK_CNT_INI;
    __asm("CPSIE i");

    LOGGER_INFO("Done initializing `app`");
}

void app_update(void)
{
    uint32_t index;
    bool b_time_update_required = false;
    uint32_t cycle_counter_time_us;

    __asm("CPSID i");
    if (G_APP_TICK_CNT_INI < g_app_tick_cnt)
    {
        g_app_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    /* Check if it's time to run tasks */
    while (b_time_update_required)
    {
        /* Update App Counter */
        g_app_cnt++;
        g_app_runtime_us = 0;

        /* Go through the task arrays */
        for (index = 0; TASK_QTY > index; index++)
        {
            cycle_counter_reset();

            /* Run task_x_update */
            (*task_cfg_list[index].task_update)(task_cfg_list[index].parameters);

            cycle_counter_time_us = cycle_counter_get_time_us();

            /* Update variables */
            g_app_runtime_us += cycle_counter_time_us;

            if (g_app_WCET_us < g_app_runtime_us) {
                g_app_WCET_us = g_app_runtime_us;
                LOGGER_INFO("app worst execution time (WCET): %lu us", g_app_WCET_us);
            }

            if (task_dta_list[index].WCET < cycle_counter_time_us)
            {
                task_dta_list[index].WCET = cycle_counter_time_us;
            }
        }

        __asm("CPSID i");
        if (G_APP_TICK_CNT_INI < g_app_tick_cnt)
        {
            g_app_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");
    }
}

void HAL_SYSTICK_Callback(void)
{
    /* Update Tick Counters */
    g_app_tick_cnt++;
    g_task_sensor_tick_cnt++;
    g_task_system_tick_cnt++;
    g_task_actuator_tick_cnt++;
}
