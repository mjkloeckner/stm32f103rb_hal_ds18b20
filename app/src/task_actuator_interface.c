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

void put_event_task_actuator(task_actuator_ev_t event, task_actuator_id_t identifier)
{
    task_actuator_dta_t *p_task_actuator_dta;

    p_task_actuator_dta = &task_actuator_dta_list[identifier];
    p_task_actuator_dta->event = event;
    p_task_actuator_dta->flag = true;
}
