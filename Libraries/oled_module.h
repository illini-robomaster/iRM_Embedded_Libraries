/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-30
 * @file    oled_module.h
 * @brief   RM 2018 OLED Module Support
 * @log     2018-05-30 nickelliang
 */

#ifndef _OLED_MODULE_H_
#define _OLED_MODULE_H_

#include "bsp_oled.h"
#include <stdlib.h>

/* How many screen of text will be stored in Text MEM */
#define TEXT_MODE_MAX_SCREEN    4
#define TEXT_MODE       ON

/*
 * Measured 5D button resistence and corresponding 12bit ADC value
 * 3.3V, 100k in series with:
 * Middle   R 0     0.000V  ADC 0
 * Left     R 26.7k 0.683V  ADC 848
 * Right    R 69.8k 1.395V  ADC 1731
 * Up       R 133k  1.654V  ADC 2458
 * Down     R 287k  2.643V  ADC 3280
 * Static   R 732k  3.223V  ADC 4000
 *      3.3V|----[R 100k]----[ADC point]----[R var]----|GND
 * Note these values are not the same as DJI's datasheet. All data
 * are based on our measurement of ~5000 data and take the mean
 * value as the actual ADC value, then find a legit similar resistor
 * (e.g. we calculate 26.64k resistence and a legit resistor would
 * be 26.7k in production), and calculate the theoretical ADC value
 * base on that resistence.
 */

 #define BUTTON_MIDDLE   0
 #define BUTTON_LEFT     848
 #define BUTTON_RIGHT    1731
 #define BUTTON_UP       2458
 #define BUTTON_DOWN     3280
 #define BUTTON_STATIC   4000
 #define BUTTON_LIMIT    128     // 0.1V tolerence

/*   Not accessable      Actual Screen       Not accessable
 *  |-------2------|-----------128----------|-------2------|
 *  |   X_START    |         X_WIDTH        |              |
 *  |              |X_MIN|----X_MID---|X_MAX|Y_MIN         |
 *  |       Y_HIGHT|     | 126*60 used|     |Y_MID         |
 *  |          64  |     |------------|     |Y_MAX         |
 *  |--------------|------------------------\--------------|
 *  SH1106G Driver IC support 132 * 64 oled screen, but the screen
 *  itself is a 128 * 64 screen, so the starting lower address
 *  should be 2. In order to support 6 * 12 ASCII character, I added
 *  a 1 pixel boarder on the top and bottom, and a 2 pixel boarder
 *  on the left and right. Hence we can display 21 * 5 = 105 chars
 *  on the screen, and the actual in use screen is 126 * 60.
 */

#define OLED_X_WIDTH    128
#define OLED_X_START    2   // Start of hardware mem due to display model
#define OLED_X_MIN      1
#define OLED_X_MID      (OLED_X_WIDTH / 2)
#define OLED_X_MAX      (OLED_X_WIDTH - OLED_X_MIN)

#define OLED_Y_HIGHT    64
#define OLED_Y_START    0
#define OLED_Y_MIN      2
#define OLED_Y_MID      (OLED_Y_HIGHT / 2)
#define OLED_Y_MAX      (OLED_Y_HIGHT - OLED_Y_MIN)

#define OLED_BYTE       8
#define OLED_PAGE       (OLED_Y_HIGHT / OLED_BYTE)

#define CHAR_WIDTH      6
#define CHAR_HIGHT      12
#define CHAR_PER_LINE   (OLED_X_WIDTH / CHAR_WIDTH)
#define CHAR_MAX_LINES  (OLED_Y_HIGHT / CHAR_HIGHT)
#define CHAR_X_OFFSET   OLED_X_MIN
#define CHAR_X_GAP      0
#define CHAR_Y_OFFSET   OLED_Y_MIN
#define CHAR_Y_GAP      0
#define CHAR_MAX_PRINT  (CHAR_PER_LINE * CHAR_MAX_LINES)

typedef enum {
    OLED_OFF = 0,
    OLED_ON  = 1,
    OLED_INVERTED,
} oled_pixel_status_t;

typedef enum {
    PRESS_MIDDLE,
    PRESS_LEFT,
    PRESS_RIGHT,
    PRESS_UP,
    PRESS_DOWN,
    PRESS_STATIC,
    PRESS_ERROR,
} oled_button_status_t;

void oled_module_init(void);

void oled_set_pos(uint8_t x, uint8_t page);

void oled_display_on(void);

void oled_display_off(void);

void oled_refresh_gram(void);

void oled_clear(oled_pixel_status_t status);

void oled_clean(void);

void oled_display_tram(void);

void oled_display_graphic(const unsigned char *graphic);

void oled_drawpoint(uint8_t x, uint8_t y, oled_pixel_status_t status);

void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_pixel_status_t status);

void oled_drawrectangle(uint8_t x_min, uint8_t y_min, uint8_t x_max, uint8_t y_max, oled_pixel_status_t status);

void oled_putc(int8_t row, uint8_t col, uint8_t chr, oled_pixel_status_t status);

void oled_putc_tram(int8_t row, uint8_t col, uint8_t chr);

uint8_t oled_puts(int8_t *row, uint8_t *col, uint8_t *chr, oled_pixel_status_t status);

void oled_printf(const char *fmt, ...);

static void oled_cursor_reset(void);

void oled_cursor_down(int8_t *row, uint8_t *col);

void oled_cursor_up(int8_t *row, uint8_t *col);

void oled_scroll_down(int8_t *row);

void oled_scroll_up(int8_t *row);

void oled_highlight_row(int8_t row);

void oled_highlight_char(int8_t row, uint8_t col);

void oled_highlight_chars(int8_t row, uint8_t col_start, uint8_t col_end);

void oled_irm_logo(void);

oled_button_status_t oled_get_button_status(void);

#endif
