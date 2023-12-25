/**
 * Clock configuration
 */

#include "at32f435_437_clock.h"

void system_clock_config(void) {
    crm_reset();
    crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

    pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);
    flash_clock_divider_set(FLASH_CLOCK_DIV_3);

    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
    while (crm_hext_stable_wait() == ERROR) {
    }

    crm_pll_config(CRM_PLL_SOURCE_HEXT, 144, 1, CRM_PLL_FR_4);  // Frequency of CPU is 288MHz

    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
    while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) {
    }

    crm_ahb_div_set(CRM_AHB_DIV_1);
    crm_apb2_div_set(CRM_APB2_DIV_2);  // Frequency of APB2 is 144 MHz
    crm_apb1_div_set(CRM_APB1_DIV_2);  // Frequency of APB1 is 144 MHz

    crm_auto_step_mode_enable(TRUE);
    crm_sysclk_switch(CRM_SCLK_PLL);
    while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) {
    }

    crm_auto_step_mode_enable(FALSE);
    system_core_clock_update();
}
