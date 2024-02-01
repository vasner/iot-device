/**
 * Linux named pipe (aka FIFO) based console interface and commands implementation
 */
#ifndef PIPE_CONSOLE_H
#define PIPE_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "console.h"

#define PIPE_CONSOLE_RX_FIFO_NAME ("iot_rx_fifo")
#define PIPE_CONSOLE_TX_FIFO_NAME ("iot_tx_fifo")

void pipe_console_init(bool is_duplicate_tx_to_stdout);
void pipe_console_deinit(void);
void pipe_console_run(void);

#ifdef __cplusplus
}
#endif

#endif // PIPE_CONSOLE_H
