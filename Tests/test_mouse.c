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

static void clip_to_range(int16_t *val, int16_t low, int16_t high) {
    assert(low < high);
    if (*val > high)
        *val = high;
    else if (*val < low)
        *val = low;
}

void test_mouse() {
    dbus_t *rc = dbus_get_struct();
    motor_t     m_yaw, m_pitch;
    pid_ctl_t   pid_yaw, pid_pitch;
    
    init_yaw(&m_yaw, &pid_yaw);
    init_pitch(&m_pitch, &pid_pitch);

    int16_t yaw_ang = 6000, pitch_ang = 5500;
    while (1) {
        yaw_ang -= rc->mouse.x;
        pitch_ang -= rc->mouse.y;
        m_yaw.out = pid_calc(&pid_yaw, yaw_ang);
        m_pitch.out = pid_calc(&pid_pitch, pitch_ang);
        set_motor_output(&m_yaw, &m_pitch, NULL, NULL);

        HAL_Delay(5);
    }
}
