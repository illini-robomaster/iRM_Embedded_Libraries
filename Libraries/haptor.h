#ifndef _HAPTOR_H_
#define _HAPTOR_H_

#include "motor.h"
#include "pid.h"
#include <inttypes.h>

/**
 * @enum    haptor_state_t
 * @var TOP     the state when haptor reaches its upper limit
 * @var BOTTOM  the state when haptor reaches its lower limit
 * @var MIDDLE  the state when haptor is in between its two limits
 */
typedef enum {
    TOP,
    MIDDLE,
    BOTTOM,
}   haptor_state_t;

/**
 * @brief initialize haptor for engineering bot
 */
void init_haptor();

/**
 * @brief lower the haptor
 * @note the haptor will be controlled by a speed / speed pid and
 *       it will stop when it hits anything
 */
void haptor_down();

/**
 * @brief lift the haptor
 * @note the haptor will be controlled by a speed / speed pid and
 *       it will stop when it hits anything
 */
void haptor_up();

#endif
