/**
 * BMP280, Digital Pressure Sensor SPI inteface implementation
 */

#include "bmp280_spi.h"

#include "platform.h"

void bmp280_spi_init(void) {
    crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    // SPI2 SCK
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_pins = GPIO_PINS_1;
    gpio_init(GPIOD, &gpio_init_struct);
    gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE1, GPIO_MUX_6);

    // SPI2 MISO
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_init_struct.gpio_pins = GPIO_PINS_2;
    gpio_init(GPIOC, &gpio_init_struct);
    gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_5);

    // SPI2 MOSI
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_init_struct.gpio_pins = GPIO_PINS_4;
    gpio_init(GPIOD, &gpio_init_struct);
    gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE4, GPIO_MUX_6);

    // SPI2 CS
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_pins = GPIO_PINS_0;
    gpio_init(GPIOD, &gpio_init_struct);
    gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE0, GPIO_MUX_7);

    spi_init_type spi_init_struct;
    spi_default_para_init(&spi_init_struct);
    spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
    spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
    spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_16;  // 9MHz
    spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
    spi_init_struct.frame_bit_num = SPI_FRAME_16BIT;
    spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_HIGH;
    spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
    spi_init_struct.cs_mode_selection = SPI_CS_HARDWARE_MODE;
    spi_init(SPI2, &spi_init_struct);
    spi_enable(SPI2, TRUE);
}

void bmp280_spi_write_reg(void* ctx, uint16_t reg) {
    (void)ctx;
    while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET) {}
    spi_i2s_data_transmit(SPI2, reg);
}

uint8_t bmp280_spi_read_reg(void* ctx, uint16_t reg) {
    (void)ctx;
    while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET) {}
    spi_i2s_data_transmit(SPI2, reg);
    while (spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET) {}
    // TODO: Compact after debug
    uint16_t result = spi_i2s_data_receive(SPI2);
    return (uint8_t)result;
}
