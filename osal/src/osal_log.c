/**
 * @file osal_log.c
 */
#include "osal_log.h"
#include "hal_uart.h"
#include "osal_queue.h"
#include "osal_task.h"
#include "osal_sched.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char text[LOG_BUFFER_SIZE + 16];
} osal_log_msg_t;

static HAL_UART *s_uart = NULL;
static osal_queue_t s_log_q = NULL;

#if OSAL_LOG_TIMESTAMP_ENABLE
static uint32_t s_osal_log_seq = 0U;
#endif

static void osal_log_tx(const char *text)
{
    size_t n;

    if (s_uart == NULL || text == NULL) {
        return;
    }
    n = strlen(text);
    if (n == 0U) {
        return;
    }
    (void)hal_uart_transmit_it(s_uart, (uint8_t *)text, n);
}

static void LogTask(void *pvParameters)
{
    osal_log_msg_t msg;

    (void)pvParameters;
    for (;;) {
        if (OSAL_OK == osal_queue_receive(s_log_q, &msg, OSAL_WAIT_FOREVER)) {
            osal_log_tx(msg.text);
        }
    }
}

void osal_log_init(HAL_UART *uart)
{
    s_uart = uart;
    if (s_log_q != NULL) {
        return;
    }

    s_log_q = osal_queue_create(OSAL_LOG_QUEUE_LEN, sizeof(osal_log_msg_t));
    if (NULL == s_log_q) {
        return;
    }

    /* Below app tasks so UART TX poll cannot starve led/hb. */
    (void)osal_task_create(LogTask, "LogTask", 256U, NULL, 1U, NULL);
}

static void osal_log_format_and_emit(const char *body)
{
    osal_log_msg_t msg;
    size_t i;
    size_t o;
    int has_nl = 0;

    if (body == NULL || body[0] == '\0') {
        return;
    }

#if OSAL_LOG_TIMESTAMP_ENABLE
    o = (size_t)snprintf(msg.text, sizeof(msg.text), "[%04lu] ",
                         (unsigned long)(s_osal_log_seq++));
#else
    o = 0U;
#endif
    if (o >= sizeof(msg.text)) {
        return;
    }

    for (i = 0U; body[i] != '\0' && o + 1U < sizeof(msg.text); i++) {
        if (body[i] == '\r') {
            continue;
        }
        msg.text[o++] = body[i];
        if (body[i] == '\n') {
            has_nl = 1;
        }
    }
    if (!has_nl && o + 1U < sizeof(msg.text)) {
        msg.text[o++] = '\n';
    }
    msg.text[o] = '\0';

    if ((s_log_q == NULL) || !osal_sched_is_running()) {
        osal_log_tx(msg.text);
        return;
    }

    (void)osal_queue_send(s_log_q, &msg, 50U);
}

void osal_log_info(const char *msg)
{
    osal_log_format_and_emit(msg);
}

void osal_log_printf(const char *fmt, ...)
{
    char body[LOG_BUFFER_SIZE];
    va_list ap;

    if (fmt == NULL) {
        return;
    }
    va_start(ap, fmt);
    (void)vsnprintf(body, sizeof(body), fmt, ap);
    va_end(ap);
    osal_log_format_and_emit(body);
}
