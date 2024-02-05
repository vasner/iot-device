/**
 * Platform related parameters and functions
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f435_437.h"

#define CRM_MODULE_ENABLED
#define TMR_MODULE_ENABLED
// #define ERTC_MODULE_ENABLED
#define GPIO_MODULE_ENABLED
// #define I2C_MODULE_ENABLED
// #define USART_MODULE_ENABLED
// #define PWC_MODULE_ENABLED
// #define CAN_MODULE_ENABLED
// #define ADC_MODULE_ENABLED
// #define DAC_MODULE_ENABLED
// #define SPI_MODULE_ENABLED
// #define EDMA_MODULE_ENABLED
// #define DMA_MODULE_ENABLED
// #define DEBUG_MODULE_ENABLED
// #define FLASH_MODULE_ENABLED
// #define CRC_MODULE_ENABLED
// #define WWDT_MODULE_ENABLED
// #define WDT_MODULE_ENABLED
// #define EXINT_MODULE_ENABLED
// #define SDIO_MODULE_ENABLED
// #define XMC_MODULE_ENABLED
#define USB_MODULE_ENABLED
// #define ACC_MODULE_ENABLED
#define MISC_MODULE_ENABLED
// #define QSPI_MODULE_ENABLED
// #define DVP_MODULE_ENABLED
// #define SCFG_MODULE_ENABLED
// #define EMAC_MODULE_ENABLED

#ifdef CRM_MODULE_ENABLED
#include "at32f435_437_crm.h"
#endif
#ifdef TMR_MODULE_ENABLED
#include "at32f435_437_tmr.h"
#endif
#ifdef ERTC_MODULE_ENABLED
#include "at32f435_437_ertc.h"
#endif
#ifdef GPIO_MODULE_ENABLED
#include "at32f435_437_gpio.h"
#endif
#ifdef I2C_MODULE_ENABLED
#include "at32f435_437_i2c.h"
#endif
#ifdef USART_MODULE_ENABLED
#include "at32f435_437_usart.h"
#endif
#ifdef PWC_MODULE_ENABLED
#include "at32f435_437_pwc.h"
#endif
#ifdef CAN_MODULE_ENABLED
#include "at32f435_437_can.h"
#endif
#ifdef ADC_MODULE_ENABLED
#include "at32f435_437_adc.h"
#endif
#ifdef DAC_MODULE_ENABLED
#include "at32f435_437_dac.h"
#endif
#ifdef SPI_MODULE_ENABLED
#include "at32f435_437_spi.h"
#endif
#ifdef DMA_MODULE_ENABLED
#include "at32f435_437_dma.h"
#endif
#ifdef DEBUG_MODULE_ENABLED
#include "at32f435_437_debug.h"
#endif
#ifdef FLASH_MODULE_ENABLED
#include "at32f435_437_flash.h"
#endif
#ifdef CRC_MODULE_ENABLED
#include "at32f435_437_crc.h"
#endif
#ifdef WWDT_MODULE_ENABLED
#include "at32f435_437_wwdt.h"
#endif
#ifdef WDT_MODULE_ENABLED
#include "at32f435_437_wdt.h"
#endif
#ifdef EXINT_MODULE_ENABLED
#include "at32f435_437_exint.h"
#endif
#ifdef SDIO_MODULE_ENABLED
#include "at32f435_437_sdio.h"
#endif
#ifdef XMC_MODULE_ENABLED
#include "at32f435_437_xmc.h"
#endif
#ifdef ACC_MODULE_ENABLED
#include "at32f435_437_acc.h"
#endif
#ifdef MISC_MODULE_ENABLED
#include "at32f435_437_misc.h"
#endif
#ifdef EDMA_MODULE_ENABLED
#include "at32f435_437_edma.h"
#endif
#ifdef QSPI_MODULE_ENABLED
#include "at32f435_437_qspi.h"
#endif
#ifdef SCFG_MODULE_ENABLED
#include "at32f435_437_scfg.h"
#endif
#ifdef EMAC_MODULE_ENABLED
#include "at32f435_437_emac.h"
#endif
#ifdef DVP_MODULE_ENABLED
#include "at32f435_437_dvp.h"
#endif
#ifdef USB_MODULE_ENABLED
#include "at32f435_437_usb.h"
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

#endif  // PLATFORM_H