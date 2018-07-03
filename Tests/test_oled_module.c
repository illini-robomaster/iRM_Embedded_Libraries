/**
 * @author  Nickel_Liang <nickelliang>
 * @date    2018-05-30
 * @file    test_oled_module.c
 * @brief   Test RM2018 OLED module
 * @log     2018-05-30 nickelliang
 */

#include "test_oled_module.h"

uint8_t test_oled_module(void) {
    // oled_module_init();
    oled_clear(OLED_OFF);
    // test_oled_display();
    test_oled_button();
    return 1;
}

void test_oled_button_raw(void) {
    uint16_t button;
    while (1) {
        button = oled_button_get_val();
        oled_clear(OLED_OFF);
        oled_printf("Button value %u\n", button);
        // print("Button value %u.\r\n", button);
    }
}

void test_oled_button(void) {
    uint32_t count;
    for (count = 0; count < 100000; count++) {
        oled_button_status_t button_state = oled_get_button_status();
        if (button_state == PRESS_MIDDLE) {
            // print("MIDDLE pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("MIDDLE pressed.\n");
        }
        else if (button_state == PRESS_LEFT) {
            // print("LEFT pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("LEFT pressed.\n");
        }
        else if (button_state == PRESS_RIGHT) {
            // print("RIGHT pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("RIGHT pressed.\n");
        }
        else if (button_state == PRESS_UP) {
            // print("UP pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("UP pressed.\n");
        }
        else if (button_state == PRESS_DOWN) {
            // print("DOWN pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("DOWN pressed.\n");
        }
        else if (button_state == PRESS_STATIC) {
            // print("STATIC pressed.\r\n");
            // oled_clear(OLED_OFF);
            oled_printf("STATIC pressed.\n");
        }
        else {
            // bsp_error_handler(__FUNCTION__, __LINE__, "ADC error.");
            // oled_clear(OLED_OFF);
            oled_printf("ERROR.\n");
        }
    }
}

void test_oled_display(void) {
    oled_clear(OLED_ON);
    oled_refresh_gram();
    HAL_Delay(500);
    oled_clear(OLED_OFF);
    oled_refresh_gram();
    HAL_Delay(500);

    oled_printf("012345678901234567890");
    oled_printf("123456789012345678901");
    oled_printf("234567890123456789012");
    oled_printf("345678901234567890123");
    oled_printf("45678901234567890123");
    oled_refresh_gram();

    uint32_t count;
    oled_button_status_t button = PRESS_STATIC;
    uint8_t highlight_col = 0, highlight_row = 0;
    uint8_t x_min, x_max, y_min, y_max;
    oled_highlight_char(highlight_row, highlight_col);
    // int16_t tick = 1000;
    for (count = 0; count < 100000; count++) {
        button = oled_get_button_status();
        switch (button) {
            case PRESS_MIDDLE:
                x_min = highlight_col * (CHAR_WIDTH + CHAR_X_GAP) + CHAR_X_OFFSET;
                x_max = x_min + CHAR_WIDTH;
                y_min = highlight_row * (CHAR_HIGHT + CHAR_Y_GAP) + CHAR_Y_OFFSET;
                y_max = y_min + CHAR_HIGHT;
                oled_drawrectangle(x_min, y_min, x_max, y_max, OLED_OFF);
                oled_highlight_char(highlight_row, highlight_col);
                break;
            case PRESS_LEFT:
                if (highlight_col >= 1) {
                    oled_highlight_char(highlight_row, highlight_col);
                    highlight_col--;
                    oled_highlight_char(highlight_row, highlight_col);
                }
                break;
            case PRESS_RIGHT:
                if (highlight_col <= CHAR_PER_LINE - 2) {
                    oled_highlight_char(highlight_row, highlight_col);
                    highlight_col++;
                    oled_highlight_char(highlight_row, highlight_col);
                }
                break;
            case PRESS_UP:
                if (highlight_row >= 1) {
                    oled_highlight_char(highlight_row, highlight_col);
                    highlight_row--;
                    oled_highlight_char(highlight_row, highlight_col);
                }
                break;
            case PRESS_DOWN:
                if (highlight_row <= CHAR_MAX_LINES - 2) {
                    oled_highlight_char(highlight_row, highlight_col);
                    highlight_row++;
                    oled_highlight_char(highlight_row, highlight_col);
                }
                break;
            default:
                // if (tick <= 0) {
                //     tick = 1000;
                //     oled_highlight_char(highlight_row, highlight_col);
                // }
                break;
        }
        oled_refresh_gram();
        // tick--;
        HAL_Delay(50);
    }
}
