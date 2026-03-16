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
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

#define G_TASK_SYS_CNT_INI       0ul
#define G_TASK_SYS_TICK_CNT_INI  0ul

#define G_LED_PERIOD_MS 1000ul
#define G_LED_T_ON_MS   500ul

task_system_dta_t task_system_dta = {
    G_LED_PERIOD_MS, ST_SYS_LED_ON
};

#define SYSTEM_DTA_QTY (sizeof(task_system_dta)/sizeof(task_system_dta_t))

void task_system_statechart(void);

uint32_t g_task_system_cnt;

// this is updated every ms by the HAL_SysTick
volatile uint32_t g_task_system_tick_cnt;

void task_system_init(void *parameters)
{
    task_system_dta_t *p_task_system_dta;
    task_system_st_t state;
    // task_system_ev_t event;
    // bool b_event;

    LOGGER_INFO("Initializing `task_system`...");

    g_task_system_cnt = G_TASK_SYS_CNT_INI;

    init_queue_event_task_system();

    p_task_system_dta = &task_system_dta;

    state = ST_SYS_LED_ON;
    p_task_system_dta->state = state;

    LOGGER_INFO("Done initializing `task_system`");
}

void task_system_update(void *parameters)
{
    bool b_time_update_required = false;

    __asm("CPSID i");
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
    {
        g_task_system_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    while (b_time_update_required)
    {
        // increment task tick counter
        g_task_system_cnt++;

        task_system_statechart();

        __asm("CPSID i");
        if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
        {
            g_task_system_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");
    }
}

void task_system_statechart(void)
{
    task_system_dta_t *p_task_system_dta = &task_system_dta;

    /*
    // system events unused

    if (true == any_event_task_system())
    {
        p_task_system_dta->flag = true;
        p_task_system_dta->event = get_event_task_system();
    }
    */

    switch (p_task_system_dta->state)
    {
        case ST_SYS_LED_ON:

            if (G_LED_T_ON_MS < g_task_system_cnt)
            {
                put_event_task_actuator(EV_LED_OFF, ID_LED_A);
                p_task_system_dta->state = ST_SYS_LED_OFF;
            }

            break;

        case ST_SYS_LED_OFF:

            if (G_LED_PERIOD_MS < g_task_system_cnt)
            {
                g_task_system_cnt = G_TASK_SYS_CNT_INI;
                put_event_task_actuator(EV_LED_ON, ID_LED_A);
                p_task_system_dta->state = ST_SYS_LED_ON;
            }

            break;

        default:

            p_task_system_dta->tick  = 0;
            p_task_system_dta->state = ST_SYS_LED_ON;

            break;
    }
}
