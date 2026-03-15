/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
#define TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* System Statechart - State Transition Table
 *
 * +----------------+-------+---------------------------+----------------+-----------------------------+
 * | Current state  | Event | [Guard]                   | Next state     | Actions                     |
 * |                |       |                           |                |                             |
 * +================+=======+===========================+================+=============================+
 * | ST_SYS_LED_ON  |       | [tick < LED_PERIOD_MS*    | ST_SYS_LED_OFF | put_event_actuator(LED_OFF) |
 * |                |       |          LED_PERCENT_ON]  |                |                             |
 * +----------------+-------+---------------------------+----------------+-----------------------------+
 * | ST_SYS_LED_OFF |       | [tick > 0]                | ST_SYS_LED_ON  | tick = TICK_MAX             |
 * |                |       |                           |                | put_event_actuator(LED_ON)  |
 * +----------------+-------+---------------------------+----------------+-----------------------------+
 *
 */

typedef enum task_system_ev {
    EV_ST_BLANK
} task_system_ev_t;

typedef enum task_system_st {
    ST_SYS_LED_ON,
    ST_SYS_LED_OFF
} task_system_st_t;

typedef struct {
    uint32_t         tick;
    task_system_st_t state;
    task_system_ev_t event;
    bool             flag;
} task_system_dta_t;

extern task_system_dta_t task_system_dta;

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */
