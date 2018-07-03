/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-28
 * @file    bsp_oled.h
 * @brief   Board support package for RM 2018 OLED module
 * @log     2018-05-28 nickelliang
 */

#include "bsp_oled.h"
#include "bsp_config.h"

/**
 * @note    To enable this module, you will have to set the corresponding
 *          pin labels in cubemx
 */
#if defined(OLED_DC_GPIO_Port) && \
    defined(OLED_DC_Pin) && \
    defined(OLED_RST_GPIO_Port) && \
    defined(OLED_RST_Pin) && \
    defined(BSP_OLED_PORT)

adc_t bsp_oled_adc;

void oled_write_byte(uint8_t data, uint8_t cmd) {
    if (cmd != OLED_CMD)
        OLED_CMD_SET();
    else
        OLED_CMD_CLR();
    HAL_SPI_Transmit(&BSP_OLED_PORT, &data, 1, 10);
}

void oled_write_bytes(uint8_t *data, uint16_t length, uint8_t cmd) {
    if (cmd != OLED_CMD)
        OLED_CMD_SET();
    else
        OLED_CMD_CLR();
    HAL_SPI_Transmit(&BSP_OLED_PORT, data, length, 50);
}

void oled_display_init(void) {
    OLED_RST_CLR();
    HAL_Delay(500);
    OLED_RST_SET();

    oled_write_byte(0xae, OLED_CMD);    //turn off oled panel
    oled_write_byte(0x00, OLED_CMD);    //set low column address
    oled_write_byte(0x10, OLED_CMD);    //set high column address
    oled_write_byte(0x40, OLED_CMD);    //set start line address
    oled_write_byte(0x81, OLED_CMD);    //set contrast control resigter
    oled_write_byte(0xff, OLED_CMD);    //set SEG output current brightness
    oled_write_byte(0xa1, OLED_CMD);    //set SEG/column mapping
    oled_write_byte(0xc8, OLED_CMD);    //set COM/row scan direction
    oled_write_byte(0xa6, OLED_CMD);    //set nomarl display
    oled_write_byte(0xa8, OLED_CMD);    //set multiplex display
    oled_write_byte(0x3f, OLED_CMD);    //1/64 duty
    oled_write_byte(0xd3, OLED_CMD);    //set display offset
    oled_write_byte(0x00, OLED_CMD);    //not offest
    oled_write_byte(0xd5, OLED_CMD);    //set display clock divide ratio/oscillator frequency
    oled_write_byte(0x80, OLED_CMD);    //set divide ratio
    oled_write_byte(0xd9, OLED_CMD);    //set pre-charge period
    oled_write_byte(0xf1, OLED_CMD);    //pre-charge: 15 clocks, discharge: 1 clock
    oled_write_byte(0xda, OLED_CMD);    //set com pins hardware configuration
    oled_write_byte(0x12, OLED_CMD);    //
    oled_write_byte(0xdb, OLED_CMD);    //set vcomh
    oled_write_byte(0x40, OLED_CMD);    //set vcom deselect level
    oled_write_byte(0x20, OLED_CMD);    //set page addressing mode
    oled_write_byte(0x02, OLED_CMD);    //
    oled_write_byte(0x8d, OLED_CMD);    //set charge pump enable/disable
    oled_write_byte(0x14, OLED_CMD);    //charge pump disable
    oled_write_byte(0xa4, OLED_CMD);    //disable entire dispaly on
    oled_write_byte(0xa6, OLED_CMD);    //disable inverse display on
    oled_write_byte(0xaf, OLED_CMD);    //turn on oled panel

    oled_write_byte(0xaf, OLED_CMD);    //display on
}

void oled_button_init(void) {
    adc_dma_enable(&bsp_oled_adc, &BSP_OLED_ADC, BSP_OLED_ADC_CHANNEL, 30);
}

uint16_t oled_button_get_val(void) {
    return adc_get_val(&bsp_oled_adc);
}

float oled_button_get_volt(void) {
    return adc_get_volt(&bsp_oled_adc);
}

#endif
