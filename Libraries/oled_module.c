/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-30
 * @file    oled_module.c
 * @brief   RM 2018 OLED Module Support
 * @log     2018-05-30 nickelliang
 */

#include "bsp_print.h"

#include "oled_module.h"
#include "oled_module_resource.h"

/* Graphic RAM */
static uint8_t OLED_GRAM[OLED_PAGE][OLED_X_WIDTH];
/* Text RAM */
static uint8_t OLED_TRAM[CHAR_MAX_PRINT * TEXT_MODE_MAX_SCREEN] = {'\0'};
static uint8_t OLED_PRINT_BUF[CHAR_MAX_PRINT] = {'\0'};
static uint8_t *oled_tram_start = OLED_TRAM;
static uint8_t *oled_tram_last_screen = OLED_TRAM + CHAR_MAX_PRINT * (TEXT_MODE_MAX_SCREEN - 1);
/* oled_printf support */
static int8_t cursor_row;
static uint8_t cursor_col;

/* ===== Initialization ===== */

void oled_module_init(void) {
    oled_display_init();
    oled_button_init();
    oled_irm_logo();
}

/* ===== Display utility ===== */

void oled_set_pos(uint8_t x, uint8_t page) {
    x += OLED_X_START;
    oled_write_byte((0xb0 + page), OLED_CMD);           // set page address y
    oled_write_byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);// set column high address
    oled_write_byte( (x & 0x0f), OLED_CMD);             // set column low address
}

void oled_display_on(void) {
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x14, OLED_CMD);
    oled_write_byte(0xaf, OLED_CMD);
}

void oled_display_off(void) {
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x10, OLED_CMD);
    oled_write_byte(0xae, OLED_CMD);
}

void oled_refresh_gram(void) {
    uint8_t page;
    for (page = 0; page < OLED_PAGE; page++) {
        oled_set_pos(0, page);
        oled_write_bytes(OLED_GRAM[page], OLED_X_WIDTH, OLED_DATA);
    }
}

void oled_clear(oled_pixel_status_t status) {
    if (status == OLED_ON)
        memset(*OLED_GRAM, 0xff, OLED_PAGE * OLED_X_WIDTH);
    else
        memset(*OLED_GRAM, 0x00, OLED_PAGE * OLED_X_WIDTH);
}

void oled_clean(void) {
    oled_clear(OLED_OFF);
    oled_cursor_reset();
}

void oled_display_tram(void) {
    uint8_t i, col = 0;
    int8_t row = 0;
    oled_clear(OLED_OFF);
    for (i = 0; i < CHAR_MAX_PRINT; i++) {
        if (oled_tram_start[i] != '\0')
            oled_putc(row, col, oled_tram_start[i], OLED_ON);
        col++;
        if (col >= CHAR_PER_LINE)
            oled_cursor_down(&row, &col);
    }
}

void oled_display_graphic(const unsigned char *graphic) {
    memcpy(*OLED_GRAM, graphic, OLED_PAGE * OLED_X_WIDTH);
}

/* ===== Draw utility ===== */

void oled_drawpoint(uint8_t x, uint8_t y, oled_pixel_status_t status) {
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if ((x >= OLED_X_MAX) || (x < OLED_X_MIN) || (y >= OLED_Y_MAX) || (y < OLED_Y_MIN))
        return;

    page = y / 8;
    row  = y % 8;

    if (status == OLED_ON)
        OLED_GRAM[page][x] |= 1 << row;
    else if (status == OLED_INVERTED)
        OLED_GRAM[page][x] ^= 1 << row;
    else
        OLED_GRAM[page][x] &= ~(1 << row);
}

void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, oled_pixel_status_t status) {
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2) {
        (x1 <= x2) ? (x_st = x1) : (x_st = x2);
        (x1 <= x2) ? (x_ed = x2) : (x_ed = x1);

        for (col = x_st; col <= x_ed; col++) {
            oled_drawpoint(col, y1, status);
        }
    }
    else if (x1 == x2) {
        (y1 <= y2) ? (y_st = y1) : (y_st = y2);
        (y1 <= y2) ? (y_ed = y2) : (y_ed = y1);

        for (row = y_st; row <= y_ed; row++) {
            oled_drawpoint(x1, row, status);
        }
    }
    else {
        k = (float)(((float)(y2 - y1)) / ((float)(x2 - x1)));
        b = (float)(y1 - k * x1);

        (x1 <= x2) ? (x_st = x1) : (x_st = x2);
        (x1 <= x2) ? (x_ed = x2) : (x_ed = x1);

        for (col = x_st; col <= x_ed; col++) {
            oled_drawpoint(col, (uint8_t)(col * k + b), status);
        }
    }
}

void oled_drawrectangle(uint8_t x_min, uint8_t y_min, uint8_t x_max, uint8_t y_max, oled_pixel_status_t status) {
    uint8_t x, y;
    for (y = y_min; y < y_max; y++) {
        for (x = x_min; x < x_max; x++) {
            oled_drawpoint(x, y, status);
        }
    }
}

/* @todo Add rectangle, circle, etc */

/* ===== Character support ===== */

void oled_putc(int8_t row, uint8_t col, uint8_t chr, oled_pixel_status_t status) {
    uint8_t x = col * (CHAR_WIDTH + CHAR_X_GAP) + CHAR_X_OFFSET;
    uint8_t y = row * (CHAR_HIGHT + CHAR_Y_GAP) + CHAR_Y_OFFSET;
    uint8_t temp_char, hight, pixel;
    uint8_t y0 = y;
    oled_pixel_status_t off_behavior = OLED_OFF;
    chr = chr - ' ';

    if (status != OLED_ON) {
        off_behavior = OLED_ON;
    }

    for (hight = 0; hight < CHAR_HIGHT; hight++) {
        temp_char = ASCII_1206[chr][hight];

        for (pixel = 0; pixel < OLED_BYTE; pixel++) {
            if (temp_char & 0x80)
                oled_drawpoint(x, y, status);
            else
                oled_drawpoint(x, y, off_behavior);

            temp_char <<= 1;
            y++;

            if ((y - y0) == CHAR_HIGHT) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void oled_putc_tram(int8_t row, uint8_t col, uint8_t chr) {
    oled_tram_start[row * CHAR_PER_LINE + col] = chr;
}

/* @note TEXT mode only support OLED_ON display */
uint8_t oled_puts(int8_t *row, uint8_t *col, uint8_t *chr, oled_pixel_status_t status) {
    uint8_t i = 0, cnt = 0;

    while (chr[i] != '\0') {
        if (chr[i] == '\n') {
            oled_cursor_down(row, col);
            oled_scroll_down(row);
            i++;
        }
        else {
            if (TEXT_MODE == ON)
                oled_putc_tram(*row, *col, chr[i]);
            else
                oled_putc(*row, *col, chr[i], status);
            (*col)++;
            i++;
            cnt++;

            if (*col >= CHAR_PER_LINE) {
                oled_cursor_down(row, col);
                oled_scroll_down(row);
            }
        }
    }
    if (TEXT_MODE == ON)
        oled_display_tram();

    return cnt;
}

void oled_printf(const char *fmt, ...) {
    memset(OLED_PRINT_BUF, '\0', CHAR_MAX_PRINT);
    va_list ap;

    va_start(ap, fmt);
    vsnprintf((char*)OLED_PRINT_BUF, CHAR_MAX_PRINT, fmt, ap);
    va_end(ap);

    oled_puts(&cursor_row, &cursor_col, OLED_PRINT_BUF, OLED_ON);
    oled_refresh_gram();
}

static void oled_cursor_reset(void) {
    cursor_row = 0;
    cursor_col = 0;
    oled_tram_start = OLED_TRAM;
    memset(OLED_TRAM, '\0', CHAR_MAX_PRINT * TEXT_MODE_MAX_SCREEN);
}

void oled_cursor_down(int8_t *row, uint8_t *col) {
    (*row)++;
    *col = 0;
}

void oled_cursor_up(int8_t *row, uint8_t *col) {
    (*row)--;
    *col = 0;
}

void oled_scroll_down(int8_t *row) {
    if (TEXT_MODE != ON)
        return;
    if (*row >= CHAR_MAX_LINES) {
        oled_tram_start += CHAR_PER_LINE;
        (*row)--;
        if (oled_tram_start == oled_tram_last_screen) {
            memcpy(OLED_TRAM, OLED_TRAM + CHAR_MAX_PRINT, CHAR_MAX_PRINT * (TEXT_MODE_MAX_SCREEN - 1));
            memset(OLED_TRAM + CHAR_MAX_PRINT * (TEXT_MODE_MAX_SCREEN - 1), '\0', CHAR_MAX_PRINT);
            oled_tram_start -= CHAR_MAX_PRINT;
        }
    }
}

void oled_scroll_up(int8_t *row) {
    if (TEXT_MODE != ON)
        return;
    if (*row < 0) {
        if (oled_tram_start == OLED_TRAM)
            return;
        oled_tram_start -= CHAR_PER_LINE;
        (*row)++;
    }
}

void oled_highlight_row(int8_t row) {
    uint8_t x_min = OLED_X_MIN;
    uint8_t x_max = OLED_X_MAX;
    uint8_t y_min = row * (CHAR_HIGHT + CHAR_Y_GAP) + CHAR_Y_OFFSET;
    uint8_t y_max = y_min + CHAR_HIGHT;
    oled_drawrectangle(x_min, y_min, x_max, y_max, OLED_INVERTED);
}

void oled_highlight_char(int8_t row, uint8_t col) {
    uint8_t x_min = col * (CHAR_WIDTH + CHAR_X_GAP) + CHAR_X_OFFSET;
    uint8_t x_max = x_min + CHAR_WIDTH;
    uint8_t y_min = row * (CHAR_HIGHT + CHAR_Y_GAP) + CHAR_Y_OFFSET;
    uint8_t y_max = y_min + CHAR_HIGHT;
    oled_drawrectangle(x_min, y_min, x_max, y_max, OLED_INVERTED);
}

void oled_highlight_chars(int8_t row, uint8_t col_start, uint8_t col_end) {
    uint8_t x_min = col_start * (CHAR_WIDTH + CHAR_X_GAP) + CHAR_X_OFFSET;
    uint8_t x_max = x_min + CHAR_WIDTH * (col_end - col_start + 1);
    uint8_t y_min = row * (CHAR_HIGHT + CHAR_Y_GAP) + CHAR_Y_OFFSET;
    uint8_t y_max = y_min + CHAR_HIGHT;
    oled_drawrectangle(x_min, y_min, x_max, y_max, OLED_INVERTED);
}

/* ===== Graphics ===== */

void oled_irm_logo(void) {
    oled_display_graphic(LOGO_IRM);
    oled_refresh_gram();
}

/* ===== 5D Button ===== */

oled_button_status_t oled_get_button_status(void) {
    uint16_t button = oled_button_get_val();
    if (abs(button - BUTTON_MIDDLE) < BUTTON_LIMIT)
        return PRESS_MIDDLE;
    else if (abs(button - BUTTON_LEFT) < BUTTON_LIMIT)
        return PRESS_LEFT;
    else if (abs(button - BUTTON_RIGHT) < BUTTON_LIMIT)
        return PRESS_RIGHT;
    else if (abs(button - BUTTON_UP) < BUTTON_LIMIT)
        return PRESS_UP;
    else if (abs(button - BUTTON_DOWN) < BUTTON_LIMIT)
        return PRESS_DOWN;
    else if (abs(button - BUTTON_STATIC) < BUTTON_LIMIT)
        return PRESS_STATIC;
    else
        return PRESS_ERROR;
}
