/**
 * BME280, Combined humidity and pressure sensor SPI inteface implementation
 */
#ifndef BME280_SPI_H
#define BME280_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void bme280_spi_init(void);
void bme280_spi_write_reg(void* ctx, uint16_t reg);
uint8_t bme280_spi_read_reg(void* ctx, uint16_t reg);

#ifdef __cplusplus
}
#endif

#endif  // BME280_SPI_H
