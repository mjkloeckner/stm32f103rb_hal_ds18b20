/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef TASK_INC_TASK_SENSOR_ATTRIBUTE_H_
#define TASK_INC_TASK_SENSOR_ATTRIBUTE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Sensor Statechart - State Transition Table
 *
 * +---------------+----------------------+-------------+--------------+--------------------------+
 * | Current state | Event                | [Guard]     | Next state   | Actions                  |
 * +===============+======================+=============+==============+==========================+
 * |               |                      | [tick == 0] | ST_SEN_BUSY  | DS18B20_Read_temp()      |
 * + ST_SEN_READY  +----------------------+-------------+--------------+--------------------------+
 * |               |                      |             | ST_SEN_READY | tick--                   |
 * +---------------+----------------------+-------------+--------------+--------------------------+
 * | ST_SEN_BUSY   | EV_DS18B20_CONV_DONE |             | ST_SEN_READY | tick =  TEMP_READ_PERIOD |
 * +---------------+----------------------+-------------+--------------+--------------------------+
 *
 */

/* Events to excite Task Sensor */
typedef enum {
    EV_DS18B20_CONV_DONE
} task_sensor_ev_t;

/* States of Task Sensor */
typedef enum {
    ST_SEN_READY,
    ST_SEN_BUSY
} task_sensor_st_t;

/* Identifier of Task Sensor */
typedef enum {
    ID_DS18B20
} task_sensor_id_t;

typedef struct {
    task_sensor_id_t identifier;
    UART_HandleTypeDef huart;
} task_sensor_cfg_t;

typedef struct {
    uint32_t tick;
    task_sensor_st_t state;
    task_sensor_ev_t event;
} task_sensor_dta_t;

extern task_sensor_dta_t task_sensor_dta_list[];

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */
