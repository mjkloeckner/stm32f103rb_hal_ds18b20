/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */


#ifndef TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_
#define TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Actuator Statechart - State Transition Table
 *
 * +--------------+--------------+---------+--------------+----------------+
 * | Current      | Event        |         | Next         |                |
 * | State        | (Parameters) | [Guard] | State        | Actions        |
 * +==============+==============+=========+==============+================+
 * |              | EV_LED_ON    |         | ST_LED_BLINK | turn_led_on()  |
 * | ST_LED_BLINK +--------------+         +--------------+----------------+
 * |              | EV_LED_OFF   |         | ST_LED_BLINK | turn_led_off() |
 * +--------------+--------------+---------+--------------+----------------+
 *
 */

/* Events to excite Task Actuator */
typedef enum task_actuator_ev {
    EV_LED_ON,
    EV_LED_OFF
} task_actuator_ev_t;

/* States of Task Actuator */
typedef enum task_actuator_st {
    ST_LED_BLINK
} task_actuator_st_t;

typedef enum task_actuator_id {
    ID_LED_A
} task_actuator_id_t;

typedef struct {
    task_actuator_id_t identifier;
    GPIO_TypeDef       *gpio_port;
    uint16_t           pin;
    GPIO_PinState      led_on;
    GPIO_PinState      led_off;
} task_actuator_cfg_t;

typedef struct {
    task_actuator_st_t state;
    task_actuator_ev_t event;
    bool               flag;
} task_actuator_dta_t;

extern task_actuator_dta_t task_actuator_dta_list[];

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_ */
