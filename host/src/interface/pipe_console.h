/**
 * Linux named pipe (aka FIFO) based console interface and commands implementation
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void pipe_console_init(bool is_duplicate_tx_to_stdout);
void pipe_console_deinit(void);
void pipe_console_run(void);

#ifdef __cplusplus
}
#endif
