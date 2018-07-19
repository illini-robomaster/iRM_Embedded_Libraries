#include "bsp_pneumatic.h"

#ifdef HERO

void pneumatic_extend(GPIO_TypeDef* gpio_port_, uint16_t gpio_pin_) {
    // for Type A dev board. 24V MOSFET power control are in the same GPIO group in CubeMX.
    HAL_GPIO_WritePin(gpio_port_, gpio_pin_, GPIO_PIN_SET);
}

void pneumatic_reset(GPIO_TypeDef* gpio_port_, uint16_t gpio_pin_) {
    // for Type A dev board. 24V MOSFET power control are in the same GPIO group in CubeMX.
    HAL_GPIO_WritePin(gpio_port_, gpio_pin_, GPIO_PIN_RESET);
}

#endif
