#include "task_motion.h"

//static osEvent motion_event;
osThreadId motion_task_handle;

void motion_task_create(void) {
    print("Function called\r\n");
    osThreadDef(motion_task_name, motion_task, osPriorityAboveNormal, 0, 256);
    motion_task_handle = osThreadCreate(osThread(motion_task_name), NULL);
    if (motion_task_handle == NULL)
        print("motion task create failed.\r\n");
    else
        print("motion task created.\r\n");
}

float get_gimbal_yaw_angle(motor_t *gimbal_motor) {
    float gimbal_yaw_angle = get_motor_angle(gimbal_motor) * (360.0 / 8192.0);
    // some operations
    return gimbal_yaw_angle;
}

chassis_t *chassis_task_init(void) {
    chassis_t *ret = (chassis_t*)malloc(sizeof(chassis_t) * 4);
    print("chassis task initing\r\n");
    chassis_init(ret);
    return ret;
}

gimbal_t *gimbal_task_init(void) {
    gimbal_t *ret = (gimbal_t*)malloc(sizeof(gimbal_t));
    gimbal_init(ret);
    return ret;
}

void motion_task(void const *argu) {
    print("MOTION TASK STARTED\r\n");
    gimbal_t *my_gimbal = gimbal_task_init();
    chassis_t *my_chassis = chassis_task_init();
    dbus_t* rc = dbus_get_struct();
    while (1) {
        gimbal_update(my_gimbal);
        int16_t yaw_astray = get_motor_angle(my_gimbal->yaw->motor) - my_gimbal->yaw_middle; // how far
        float yaw_astray_in_rad = yaw_astray * MOTOR_2_RAD;
        // chassis part; should be generic
        calc_keyboard_move(my_chassis, rc, yaw_astray_in_rad);
        calc_gimbal_compensate(my_chassis, yaw_astray_in_rad);
        run_chassis(my_chassis);
        // gimbal part; customization happens at IMU
        int16_t observed_absolute_gimbal_yaw = yaw_astray + imuBoard.angle[YAW] * DEG_2_MOTOR;
        observed_absolute_gimbal_yaw = observed_absolute_gimbal_yaw % ANGLE_RANGE_6623;
        //gimbal_mouse_move(my_gimbal, rc, observed_absolute_gimbal_yaw);
        //run_gimbal(my_gimbal);
        osDelay(MOTION_CYCLE);
    }
}
