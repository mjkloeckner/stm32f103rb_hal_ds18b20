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
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

#define G_TASK_ACT_CNT_INIT     0ul
#define G_TASK_ACT_TICK_CNT_INI 0ul

#define DEL_LED_XX_PUL          250ul
#define DEL_LED_XX_BLI          500ul
#define DEL_LED_XX_MIN          0ul

const task_actuator_cfg_t task_actuator_cfg_list[] = {
    {ID_LED_A, LED_A_PORT, LED_A_PIN, LED_A_ON, LED_A_OFF}
};

#define ACTUATOR_CFG_QTY (sizeof(task_actuator_cfg_list)/sizeof(task_actuator_cfg_t))

task_actuator_dta_t task_actuator_dta_list[] = {
    {ST_LED_BLINK}
};

#define ACTUATOR_DTA_QTY (sizeof(task_actuator_dta_list)/sizeof(task_actuator_dta_t))

void task_actuator_statechart(void);

uint32_t g_task_actuator_cnt;

// this variable is incremented by 1 on every ms by HAL_SysTick
volatile uint32_t g_task_actuator_tick_cnt;

void task_actuator_init(void *parameters)
{
    uint32_t index;
    const task_actuator_cfg_t *p_task_actuator_cfg;
    task_actuator_dta_t *p_task_actuator_dta;
    task_actuator_st_t state;
    // task_actuator_ev_t event;
    // bool b_event;

    LOGGER_INFO("Initializing `task_actuator`...");

    for (index = 0; ACTUATOR_DTA_QTY > index; index++)
    {
        p_task_actuator_cfg = &task_actuator_cfg_list[index];
        p_task_actuator_dta = &task_actuator_dta_list[index];

        state = ST_LED_BLINK;
        p_task_actuator_dta->state = state;

        HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
    }

    LOGGER_INFO("Done initializing `task_actuator`");
}

void task_actuator_update(void *parameters)
{
    bool b_time_update_required = false;

    __asm("CPSID i");
    if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_tick_cnt)
    {
        g_task_actuator_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    while (b_time_update_required)
    {
        g_task_actuator_cnt++;

        task_actuator_statechart();

        __asm("CPSID i");
        if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_tick_cnt)
        {
            g_task_actuator_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");
    }
}

void task_actuator_statechart(void)
{
    uint32_t index;
    const task_actuator_cfg_t *p_task_actuator_cfg;
    task_actuator_dta_t *p_task_actuator_dta;

    for (index = 0; ACTUATOR_DTA_QTY > index; index++)
    {
        /* Update Task Actuator Configuration & Data Pointer */
        p_task_actuator_cfg = &task_actuator_cfg_list[index];
        p_task_actuator_dta = &task_actuator_dta_list[index];

        switch (p_task_actuator_dta->state)
        {
            case ST_LED_BLINK:

                if (true == p_task_actuator_dta->flag)
                {
                    switch (p_task_actuator_dta->event)
                    {
                        case EV_LED_ON:

                            p_task_actuator_dta->flag = false;
                            HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port,
                                    p_task_actuator_cfg->pin,
                                    p_task_actuator_cfg->led_on);

                        break;
                        case EV_LED_OFF:
                            p_task_actuator_dta->flag = false;
                            HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port,
                                    p_task_actuator_cfg->pin,
                                    p_task_actuator_cfg->led_off);
                        break;
                        default:
                        break;
                    }
                }
            default:
            break;
        }
    }
}
