#include "shooter.h"
#include "lib_config.h"

#ifdef HAS_SHOOTER

static flywhl_t* pwm_flywhl_init(flywhl_t *pwm_flywhl) {
#ifdef USE_PWM_FLYWHL
    if (!pwm_flywhl)
        pwm_flywhl = pvPortMalloc(sizeof(flywhl_t));

    pwm_t *pwm_l = pwm_init(NULL, FLYWHL_PWM_TIMER, FLYWHL_LEFT_CHANNEL);
    pwm_t *pwm_r = pwm_init(NULL, FLYWHL_PWM_TIMER, FLYWHL_RIGHT_CHANNEL);

    pwm_flywhl->left = pwm_motor_init(NULL, FLYWHL_TYPE, pwm_l, 1000);
    pwm_flywhl->right = pwm_motor_init(NULL, FLYWHL_TYPE, pwm_r, 1000);

    pwm_flywhl->state = FLYWHL_OFF;
#endif

    return pwm_flywhl;
}

static flywhl_t* can_flywhl_init(flywhl_t *can_flywhl) {
#ifdef USE_CAN_FLYWHL
    /* TODO: Not implemented yet */    
    can_flywhl->state = FLYWHL_OFF;
#endif

    return can_flywhl;
}

static pid_ctl_t* poker_init(pid_ctl_t* poker) {
    motor_t *m_poker = can_motor_init(NULL, POKER_ID, POKER_CAN, POKER_TYPE);

    poker = pid_init(poker, POKE, m_poker, -5000, 0, 30000, 0, 0, 6.5, 0, 0, 9000, 0);

    return poker;
}

shooter_t *shooter_init(shooter_t *shooter, shooter_type_t type) {
    if (shooter == NULL)
        shooter = pvPortMalloc(sizeof(shooter_t));

    shooter->poker = poker_init(NULL);
    shooter->type = type;

    switch (type) {
        case CAN:
            shooter->flywhl = can_flywhl_init(NULL);
            break;
        case PWM:
            shooter->flywhl = pwm_flywhl_init(NULL);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "flywheel type not supported");
            break;
    }

    return shooter;
}

void flywhl_set_output(shooter_t *shooter, int32_t output) {
    if (output)
        shooter->flywhl->state = FLYWHL_ON;
    else
        shooter->flywhl->state = FLYWHL_OFF;

    switch (shooter->type) {
        case CAN:
            /* TODO: Not yet implemented */
            break;
        case PWM:
            shooter->flywhl->left->out = output;
            shooter->flywhl->right->out = output;
            set_pwm_motor_output(shooter->flywhl->left);
            set_pwm_motor_output(shooter->flywhl->right);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "flywheel type not supported");
            break;
    }
}

void flywhl_on(shooter_t *shooter) {
    flywhl_set_output(shooter, FLYWHL_OUTPUT);
}

void flywhl_off(shooter_t *shooter) {
    flywhl_set_output(shooter, 0);
}

void poker_set_speed(shooter_t *shooter, int32_t speed) {
    shooter->poker->motor->out = pid_calc(shooter->poker, speed);
    switch (shooter->type) {
        case CAN:
            /* TODO: Not yet implemente */
            break;
        case PWM:
            set_can_motor_output(NULL, NULL, NULL, shooter->poker->motor);
            break;
        default:
            bsp_error_handler(__FUNCTION__, __LINE__, "flywheel type not supported");
            break;
    }
}

#endif
