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
#include "task_system_attribute.h"

#define EVENT_UNDEFINED (255)
#define MAX_EVENTS      (16)

struct
{
    uint32_t         head;
    uint32_t         tail;
    uint32_t         count;
    task_system_ev_t queue[MAX_EVENTS];
} queue_task_a;

void init_queue_event_task_system(void)
{
    uint32_t i;

    queue_task_a.head = 0;
    queue_task_a.tail = 0;
    queue_task_a.count = 0;

    for (i = 0; i < MAX_EVENTS; i++)
        queue_task_a.queue[i] = EVENT_UNDEFINED;
}

void put_event_task_system(task_system_ev_t event)
{
    queue_task_a.count++;
    queue_task_a.queue[queue_task_a.head++] = event;

    if (MAX_EVENTS == queue_task_a.head)
        queue_task_a.head = 0;
}

task_system_ev_t get_event_task_system(void)
{
    task_system_ev_t event;

    queue_task_a.count--;
    event = queue_task_a.queue[queue_task_a.tail];
    queue_task_a.queue[queue_task_a.tail++] = EVENT_UNDEFINED;

    if (MAX_EVENTS == queue_task_a.tail)
        queue_task_a.tail = 0;

    return event;
}

bool any_event_task_system(void)
{
    return (queue_task_a.head != queue_task_a.tail);
}
