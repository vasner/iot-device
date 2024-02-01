/**
 * Platform related parameters and functions
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#define PLATFORM_CRM_TMR_FREQ_HZ (288000000)

/**
 * Adjust the value of high speed external crystal (HEXT) used in your application.
 * Note: To avoid modifying this file each time you need to use different HEXT, you
 *      can define the HEXT value in your toolchain compiler preprocessor.
 */
#if !defined HEXT_VALUE
#define HEXT_VALUE ((uint32_t)8000000) /*!< value of the high speed external crystal in hz */
#endif

/**
 * Adjust the high speed external crystal (HEXT) startup timeout value
 */

// Time out for hext start up
#define HEXT_STARTUP_TIMEOUT ((uint16_t)0x3000)

// Value of the high speed internal clock in Hz
#define HICK_VALUE ((uint32_t)8000000)

// Value of the low speed external clock in Hz
#define LEXT_VALUE ((uint32_t)32768)

void platform_init_system_clock(void);

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TMR1_CH_IRQHandler(void);
void TMR4_GLOBAL_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H