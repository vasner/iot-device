/**
 * BMP280, Digital Pressure Sensor SPI inteface implementation
 */
#ifndef BMP280_SPI_H
#define BMP280_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void bmp280_spi_init(void);
void bmp280_spi_write_reg(void* ctx, uint16_t reg);
uint8_t bmp280_spi_read_reg(void* ctx, uint16_t reg);

#ifdef __cplusplus
}
#endif

#endif  // BMP280_SPI_H
