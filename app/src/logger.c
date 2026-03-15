/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 *
 * See file `LICENSE` for full details
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

#include "logger.h"

#if 1 == LOGGER_CONFIG_ENABLE
static char logger_msg_buffer_[LOGGER_CONFIG_MAXLEN];
char* const logger_msg = logger_msg_buffer_;
int logger_msg_len;
#endif

#if 1 == LOGGER_CONFIG_USE_SEMIHOSTING
void logger_log_print_(char* const msg)
{
    printf(msg);
    fflush(stdout);
}
#else
void logger_log_print_(char* const msg)
{
    return;
}
#endif
