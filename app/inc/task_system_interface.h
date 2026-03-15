/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef TASK_INC_TASK_SYSTEM_INTERFACE_H_
#define TASK_INC_TASK_SYSTEM_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void init_queue_event_task_system(void);
extern void put_event_task_system(task_system_ev_t event);
extern task_system_ev_t get_event_task_system(void);
extern bool any_event_task_system(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_INTERFACE_H_ */
