/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */


#ifndef TASK_INC_TASK_SENSOR_H_
#define TASK_INC_TASK_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t g_task_sensor_cnt;
extern volatile uint32_t g_task_sensor_tick_cnt;

extern void task_sensor_init(void *parameters);
extern void task_sensor_update(void *parameters);

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_H_ */
