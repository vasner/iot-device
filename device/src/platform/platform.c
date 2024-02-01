/**
 * Platform related parameters and functions
 */

#include "platform.h"

#include "at32f435_437.h"
#include "at32f435_437_crm.h"
#include "at32f435_437_pwc.h"
#include "at32f435_437_flash.h"
#include "at32f435_437_tmr.h"

void platform_init_system_clock(void) {
    crm_reset();
    crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

    pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);
    flash_clock_divider_set(FLASH_CLOCK_DIV_3);

    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
    while (crm_hext_stable_wait() == ERROR) {}

    crm_pll_config(CRM_PLL_SOURCE_HEXT, 144, 1, CRM_PLL_FR_4);

    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
    while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) {}

    crm_ahb_div_set(CRM_AHB_DIV_1);
    crm_apb2_div_set(CRM_APB2_DIV_2);
    crm_apb1_div_set(CRM_APB1_DIV_2);

    crm_auto_step_mode_enable(TRUE);
    crm_sysclk_switch(CRM_SCLK_PLL);
    while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) {}

    crm_auto_step_mode_enable(FALSE);
    system_core_clock_update();
}

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
