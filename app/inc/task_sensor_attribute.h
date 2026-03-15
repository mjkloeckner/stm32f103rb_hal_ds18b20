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

/* Sensor Statechart - State Transition Table */
/* +-------------------+----------------+-------------+-------------------+-----------------------+
 * | Current           | Event          |             | Next              |                       |
 * | State             | (Parameters)   | [Guard]     | State             | Actions               |
 * |===================+================+=============+===================+=======================|
 * |                   |                |             |                   |                       |
 * +-------------------+----------------+-------------+-------------------+-----------------------+
 */

/* Events to excite Task Sensor */
typedef enum {
    EV_BLANK
} task_sensor_ev_t;

/* States of Task Sensor */
typedef enum {
    ST_BLANK
} task_sensor_st_t;

/* Identifier of Task Sensor */
typedef enum {
    ID_BLANK
} task_sensor_id_t;

typedef struct {
    void *empty;
} task_sensor_cfg_t;

typedef struct {
    task_sensor_id_t identifier;
    task_sensor_ev_t sig;
} task_sensor_dta_t;

extern task_sensor_dta_t task_sensor_dta_list[];

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */
