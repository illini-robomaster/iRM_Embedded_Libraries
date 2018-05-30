#include "chassis.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

static void normalize_to_range(float *vx, float *vy) {
    float norm = 1;
    if (*vx != 0 || *vy != 0)
        norm = sqrt(*vx * *vx + *vy * *vy);
    if (norm > 1) {
        *vx /= norm;
        *vy /= norm;
    }
}

void chassis_init(pid_ctl_t *my_chassis[4]){
    motor_t *m_fl, *m_fr, *m_rl, *m_rr;
    pid_ctl_t *pid_fl, *pid_fr, *pid_rl, *pid_rr;
    m_fl = m_fr = m_rl = m_rr = NULL;
    pid_fl = pid_fr = pid_rl = pid_rr = NULL;

    m_fl = motor_init(NULL, FL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_fr = motor_init(NULL, FR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rl = motor_init(NULL, RL_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);
    m_rr = motor_init(NULL, RR_MOTOR_CANID, CAN1_ID, CHASSIS_MOTOR_TYPE);

    pid_fl = pid_init(NULL, CHASSIS_ROTATE, m_fl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_fr = pid_init(NULL, CHASSIS_ROTATE, m_fr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_rl = pid_init(NULL, CHASSIS_ROTATE, m_rl, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);
    pid_rr = pid_init(NULL, CHASSIS_ROTATE, m_rr, -MAX_SPEED, MAX_SPEED, chs_int_lim,
                0, 0, chs_kp, chs_ki, chs_kd, 0, chs_calc_interval, 0);

    my_chassis[CHASSIS_FL] = pid_fl;
    my_chassis[CHASSIS_FR] = pid_fr;
    my_chassis[CHASSIS_RL] = pid_rl;
    my_chassis[CHASSIS_RR] = pid_rr;
}

void calc_keyboard_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle) {
    // counterclockwise: positive
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = 0;
    float v_x = 0;
    if (rc->key.bit.W)
        v_y += 1;
    if (rc->key.bit.S)
        v_y -= 1;
    if (rc->key.bit.A)
        v_x -= 1;
    if (rc->key.bit.D)
        v_x += 1;
    normalize_to_range(&v_x, &v_y);
    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_SPEED;
    print("Tarx: %f, Tary: %f", target_x, target_y);
    my_chassis[CHASSIS_FL]->motor->target = target_x;
    my_chassis[CHASSIS_RR]->motor->target = -target_x; // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = target_y;
    my_chassis[CHASSIS_FR]->motor->target = -target_y;
}

void calc_remote_move(pid_ctl_t *my_chassis[4], dbus_t *rc, float yaw_angle) {
    yaw_angle = -yaw_angle + Q_PI;
    float v_y = rc->ch1 * 1.0 / 660;
    float v_x = rc->ch0 * 1.0 / 660;
    normalize_to_range(&v_x, &v_y);
    // rotation; change of basis matrix.
    float target_x = (v_x * cos(yaw_angle) + v_y * sin(yaw_angle)) * MAX_SPEED;
    float target_y = (-v_x * sin(yaw_angle) + v_y * cos(yaw_angle)) * MAX_SPEED;
    my_chassis[CHASSIS_FL]->motor->target = target_x;
    my_chassis[CHASSIS_RR]->motor->target = -target_x; // velocity is the same. It's just these two motors are installed in opposite direction.
    my_chassis[CHASSIS_RL]->motor->target = target_y;
    my_chassis[CHASSIS_FR]->motor->target = -target_y;
}

void calc_remote_rotate(pid_ctl_t *my_chassis[4], dbus_t *rc) {
    float speed = rc->ch2 * 1.0 / 660 * MAX_TURN_SPEED;

    add_rotation(my_chassis, speed);
}

void add_rotation(pid_ctl_t *my_chassis[4], float speed) {
    /* apply safety boundary for turning speed */
    speed = fabs(speed) <= MAX_TURN_SPEED ? speed : sign(speed) * TURNING_SPEED;

    my_chassis[CHASSIS_FL]->motor->target += speed;
    my_chassis[CHASSIS_RR]->motor->target += speed;
    my_chassis[CHASSIS_RL]->motor->target += speed;
    my_chassis[CHASSIS_FR]->motor->target += speed;
}

void calc_gimbal_compensate(pid_ctl_t *my_chassis[4], float yaw_angle) {
    if (fabsf(yaw_angle) < YAW_DEADBAND)
        return;

    if (yaw_angle > 0)
        add_rotation(my_chassis, TURNING_SPEED);    // CW
    else
        add_rotation(my_chassis, -TURNING_SPEED);   // CCW
}

void run_chassis(pid_ctl_t *my_chassis[4]){
    for (int i = 0; i < 4; ++i) {
        my_chassis[i]->motor->out = pid_calc(my_chassis[i], my_chassis[i]->motor->target);
    }
    set_motor_output(my_chassis[CHASSIS_FL]->motor, my_chassis[CHASSIS_FR]->motor,
                my_chassis[CHASSIS_RL]->motor, my_chassis[CHASSIS_RR]->motor);
}
