#include "test_mouse.h"
#include "dbus.h"
#include "motor.h"
#include "pid.h"
#include "assert.h"

static void init_yaw(motor_t *motor, pid_ctl_t *pid) {
    size_t i;
    motor_init(motor, 0x209, CAN1_ID, M6623);
    //we must send some nonzero data to initialize CAN
    for (i = 0; i < 100; i++) {
        get_motor_data(motor);
        motor->out = 1;
        set_motor_output(motor, NULL, NULL, NULL);
    }
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 5200, 6800, 2000, 500, 200, 9, 0.1, 70, 1600, 5, 0);
}

static void init_pitch(motor_t *motor, pid_ctl_t *pid) {
    size_t i = 0;
    motor_init(motor, 0x20A, CAN1_ID, M6623);
    //we must send some nonzero data to initialize CAN
    for (i = 0; i < 100; i++) {
        get_motor_data(motor);
        motor->out = 1;
        set_motor_output(NULL, motor, NULL, NULL);
    }
    pid_init(pid, GIMBAL_MAN_SHOOT, motor, 4800, 6200, 3000, 400, 100, 12, 0.2, 90, 3000, 5, 0);
}

static void init_shoot(motor_t *m_fy_left, motor_t *m_fy_right, motor_t *m_poke,
        pid_ctl_t *pid_fy_left, pid_ctl_t *pid_fy_right, pid_ctl_t *pid_poke) {
    size_t i;
    motor_init(m_poke, 0x208, CAN1_ID, M3508);
    motor_init(m_fy_left, 0x205, CAN1_ID, M3508);
    motor_init(m_fy_right, 0x206, CAN1_ID, M3508);
    pid_init(pid_poke, POKE, m_poke, -2000, 0, 80000, 0, 0, 14, 2, 0, 10000, 5, 0);
    pid_init(pid_fy_left, FLYWHEEL, m_fy_left, -4000, 0, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
    pid_init(pid_fy_right, FLYWHEEL, m_fy_right, 0, 4000, 0, 0, 0, 22, 0, 0, 3000, 5, 0);
    m_fy_left->out = m_fy_right->out = 1;

    for (i = 0; i < 100; i++) {
        get_motor_data(m_fy_right);
        get_motor_data(m_fy_left);
        get_motor_data(m_poke);
        set_motor_output(m_fy_left, m_fy_right, NULL, m_poke);
    }
}

void test_mouse() {
    dbus_t *rc = dbus_get_struct();
    motor_t     m_yaw, m_pitch, m_fy_left, m_fy_right, m_poke;
    pid_ctl_t   pid_yaw, pid_pitch, pid_fy_left, pid_fy_right, pid_poke;
    
    init_yaw(&m_yaw, &pid_yaw);
    init_pitch(&m_pitch, &pid_pitch);
    init_shoot(&m_fy_left, &m_fy_right, &m_poke,
            &pid_fy_left, &pid_fy_right, &pid_poke);

    int32_t yaw_ang = 6000, pitch_ang = 5500;
    int32_t flywheel_speed = 1000;
    int32_t poke_speed = 300;
    while (1) {
        yaw_ang -= rc->mouse.x * 0.2;
        pitch_ang -= rc->mouse.y * 0.2;
        m_yaw.out = pid_calc(&pid_yaw, yaw_ang);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
        m_poke.out = m_fy_left.out = m_fy_right.out = 0;
        if (rc->key.bit.SHIFT) {
            m_fy_left.out = pid_calc(&pid_fy_left, -flywheel_speed);
            m_fy_right.out = pid_calc(&pid_fy_right, flywheel_speed);
            if (rc->mouse.l)
                m_poke.out = pid_calc(&pid_poke, -poke_speed);
        }

        set_motor_output(&m_yaw, &m_pitch, NULL, NULL);
        set_motor_output(&m_fy_left, &m_fy_right, NULL, &m_poke);
        HAL_Delay(5);
    }
}
