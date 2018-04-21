/**
 * @author  Eric_Liang <hanghang177>
 * @date    2018-04-20
 * @file    servo.h
 * @brief   Library Level servo control
 * #log     2018-04-20 hanghang177
 */

#ifndef _SERVO_H_
#define _SERVO_H_

/**
 * @ingroup Library
 * @defgroup servo Servo
 * @{
 */

#include "bsp_pwm.h"

#define STANDARDMAX 180
#define STANDARDMIN 0

/**
 * @struct            servo_t
 * @brief             holds info for servo
 * @var tim_id        tim id
 * @var channel_id    channel id with in tim
 * @var init_pwm      pwm width to initialize to (1500 for generic servo)
 * @var min_degree    the min. degree the servo can go to
 * @var max_degree    the max. degree the servo can go to
 * @var current_pwm   the current pwm that the servo holds
 * @var current_angle the current angle that the servo is at
 */

typedef struct {
    uint8_t        tim_id;
    uint8_t        channel_id;
    uint32_t       init_pwm;
    uint32_t       min_degree;
    uint32_t       max_degree;
    uint32_t       current_pwm;
    uint32_t       current_angle;
}   servo_t;

/**
 * Servo Initialize
 * @brief initialize a servo or a PWM based ESC
 * @param servo      servo_t type servo to be initialized
 * @param tim_id     tim id for the servo channel
 * @param channel_id channel id for the servo within tim
 * @param init_pwm   the initial value for servo (Default: 1500)
 */

void servo_init(servo_t *servo,
                uint8_t tim_id, uint8_t channel_id, uint32_t init_pwm);

/**
 * Servo writeMicroseconds
 * @brief write a certain PWM(us) to the servo
 * @param servo servo_t variable
 * @param pwm   pwm pulsewidth
 */

void servo_writeMicroseconds(servo_t *servo,
                uint32_t pwm);

/**
 * Servo write
 * @brief write a certain degree to the servo
 * @param servo   servo_t variable
 * @param degrees degrees
 */

void servo_write(servo_t *servo,
                uint32_t degrees);

/**
 * Servo set max
 * @brief set the servo's max degree
 * @param servo   servo_t variable
 * @param degrees degrees
 */

void servo_setmax(servo_t *servo,
                uint32_t degrees);

/**
 * Servo set min
 * @brief set the servo's min degree
 * @param servo   servo_t variable
 * @param degrees degrees
 */

void servo_setmin(servo_t *servo,
                uint32_t degrees);

/**
 * PWM to Degrees
 * @brief convert pwm pulsewidth to degrees
 * @param  servo  servo_t variable
 * @param  pwm    pwm pulsewidth
 * @return        degrees
 */

uint32_t pwm_to_degrees(servo_t *servo, uint32_t pwm);

/**
 * Degrees to PWM
 * @brief converts degrees to pwm pulsewidth
 * @param  servo    servo_t variable
 * @param  degrees  degrees
 * @return          pwm pulsewidth
 */

uint32_t degrees_to_pwm(servo_t *servo, uint32_t pwm);

/** @} */

#endif
