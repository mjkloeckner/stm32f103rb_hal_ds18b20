/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef TASK_INC_TASK_ACTUATOR_INTERFACE_H_
#define TASK_INC_TASK_ACTUATOR_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void put_event_task_actuator(task_actuator_ev_t event, task_actuator_id_t identifier);

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_ACTUATOR_INTERFACE_H_ */
