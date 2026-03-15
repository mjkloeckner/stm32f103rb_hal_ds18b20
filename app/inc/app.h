/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef APP_INC_APP_H_
#define APP_INC_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t g_app_cnt;
extern uint32_t g_app_runtime_us;
extern uint32_t g_app_WCET_us;
extern volatile uint32_t g_app_tick_cnt;

extern void app_init(void);
extern void app_update(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_INC_APP_H_ */
