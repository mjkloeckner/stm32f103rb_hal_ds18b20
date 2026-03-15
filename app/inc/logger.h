/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LOGGER_CONFIG_ENABLE                    (1)
#define LOGGER_CONFIG_MAXLEN                    (64)
#define LOGGER_CONFIG_USE_SEMIHOSTING           (1)

#if 1 == LOGGER_CONFIG_ENABLE
#define LOGGER_LOG(...)\
    __asm("CPSID i");	/* disable interrupts*/\
    {\
        logger_msg_len = snprintf(logger_msg, (LOGGER_CONFIG_MAXLEN - 1), __VA_ARGS__);\
        logger_log_print_(logger_msg);\
    }\
    __asm("CPSIE i");	/* enable interrupts*/
#else
#define LOGGER_LOG(...)
#endif

#define LOGGER_INFO(...)\
    LOGGER_LOG("[INFO] ");\
    LOGGER_LOG(__VA_ARGS__);\
    LOGGER_LOG("\n");

#define LOGGER_ERROR(...)\
    LOGGER_LOG("[ERROR] ");\
    LOGGER_LOG(__VA_ARGS__);\
    LOGGER_LOG("\n");

#define GET_NAME(var)  #var

extern char* const logger_msg;
extern int logger_msg_len; // only for debug information

void logger_log_print_(char* const msg);

#ifdef __cplusplus
}
#endif

#endif /* INC_LOGGER_H_ */
