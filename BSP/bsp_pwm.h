/**
 * @author  Eric_Liang <hanghang177>
 * @date    2018-04-20
 * @file    bsp_pwm.h
 * @brief   Board support package for pwm servos and escs
 * #log     2018-04-20 hanghang177
 */

#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_

#include "tim.h"

/**
 * @ingroup bsp
 * @defgroup bsp_pwm BSP PWM
 * @{
 */

/**
 * A simpler name for writing PWM
 */

#define pwm_transmit __HAL_TIM_SET_COMPARE

/**
 * PWM5 init wrapper
 */

void pwm5_init(void);

/**
 * PWM5 trasmit data
 * @param  channel     Channel number in PWM5
 * @param  pulsewidth  PWM pulsewidth (keep within 1000-2000)
 */

void pwm5_transmit(uint32_t channel, uint32_t pulsewidth);

/**
 * PWM init wrapper
 * @param htim HTIM ID to use (Default: htim5)
 */

static void pwm_init(TIM_HandleTypeDef *htim);

/** @} */

#endif
