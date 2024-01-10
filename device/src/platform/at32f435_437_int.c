/**
 * Interrupt service routines
 */

#include "at32f435_437_int.h"

#include "at32f435_437_board.h"

void NMI_Handler(void) {}

void HardFault_Handler(void) {
    while (1) {
    }
}

void MemManage_Handler(void) {
    while (1) {
    }
}

void BusFault_Handler(void) {
    while (1) {
    }
}

void UsageFault_Handler(void) {
    while (1) {
    }
}

void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}
void SysTick_Handler(void) {}

// TODO: Remove or move to LED module in future
void TMR1_CH_IRQHandler(void) {
    if (tmr_flag_get(TMR1, TMR_C4_FLAG) != RESET) {
        tmr_flag_clear(TMR1, TMR_C4_FLAG);
    }
}

void TMR4_GLOBAL_IRQHandler(void) {
    if (tmr_flag_get(TMR4, TMR_C4_FLAG) != RESET) {
        tmr_flag_clear(TMR4, TMR_C4_FLAG);
        at32_led_toggle(LED4);
    }
}
