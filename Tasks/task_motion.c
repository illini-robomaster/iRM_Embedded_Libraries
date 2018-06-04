#include "task_motion.h"

osThreadId chassis_task_handle;
osThreadId gimbal_task_handle;
gimbal_t my_gimbal;
pid_ctl_t *my_chassis[4];

void motion_task_create(void) {
    taskENTER_CRITICAL();
    gimbal_init(&my_gimbal);
    chassis_init(my_chassis);
    taskEXIT_CRITICAL();
    print("Function called\r\n");
    osThreadDef(chassisTask, chassis_task, osPriorityAboveNormal, 0, 256);
    chassis_task_handle = osThreadCreate(osThread(chassisTask), NULL);
    if (chassis_task_handle == NULL)
        print("Chassis task create failed.\r\n");
    else
        print("Chassis task created.\r\n");
    osThreadDef(gimbal_task_name, gimbal_task, osPriorityAboveNormal, 0, 256);
    gimbal_task_handle = osThreadCreate(osThread(gimbal_task_name), NULL);
    if (gimbal_task_handle == NULL)
        print("Gimbal task create failed.\r\n");
    else
        print("Gimbal task created.\r\n");
}

float get_gimbal_yaw_angle(motor_t *gimbal_motor) {
    float gimbal_yaw_angle = get_motor_angle(gimbal_motor) * (360.0 / 8192.0);
    // some operations
    return gimbal_yaw_angle;
}

void chassis_task(void const *argu) {
    print("CHASSIS TASK STARTED\r\n");
    dbus_t *rc = dbus_get_struct();
    float yaw_astray_in_rad;
    int16_t yaw_astray, cur_yaw_feedback;
    uint8_t evasion_mode = 0;
    uint32_t chassis_wake_time = osKernelSysTick();
    while (1) {
        gimbal_update(&my_gimbal);
        cur_yaw_feedback = get_motor_angle(my_gimbal.yaw->motor);
        yaw_astray = cur_yaw_feedback - my_gimbal.yaw_middle; // how far
        yaw_astray_in_rad = yaw_astray * MOTOR_2_RAD;
        // TODO: replace the following three 0s with yaw_astray_in_rad
#ifdef USE_REMOTE
        calc_remote_move(my_chassis, rc, yaw_astray_in_rad);
#else
        calc_keyboard_move(my_chassis, rc, yaw_astray_in_rad);
#endif
        if (rc->key.bit.X) {
            evasive_move(my_chassis, cur_yaw_feedback, my_gimbal.yaw->motor);
        } else {
            adjust_chassis_gimbal_pos(my_chassis, my_gimbal.yaw_middle, my_gimbal.yaw->motor);
        }

        run_chassis(my_chassis);
        osDelayUntil(&chassis_wake_time, MOTION_CYCLE);
    }
}

void gimbal_task(void const *argu) {
    print("GIMBAL TASK STARTED\r\n");
    dbus_t *rc = dbus_get_struct();
    int16_t yaw_astray;
    int32_t observed_absolute_gimbal_yaw;
    uint32_t gimbal_wake_time = osKernelSysTick();
    while (1) {
        gimbal_update(&my_gimbal);
        yaw_astray = get_motor_angle(my_gimbal.yaw->motor) - my_gimbal.yaw_middle;
        observed_absolute_gimbal_yaw = (int32_t)(yaw_astray) + (int32_t)(imuBoard.angle[YAW] * DEG_2_MOTOR);
#ifdef USE_REMOTE
        gimbal_remote_move(&my_gimbal, rc, observed_absolute_gimbal_yaw);
#else
        gimbal_mouse_move(&my_gimbal, rc, observed_absolute_gimbal_yaw);
#endif
        run_gimbal(&my_gimbal);
        osDelayUntil(&gimbal_wake_time, MOTION_CYCLE);
    }
}
