/**************************************************************************
 *  Copyright (C) 2018 
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "task_motion.h"

osThreadId chassis_task_handle;
osThreadId gimbal_task_handle;

gimbal_t        my_gimbal;
pid_ctl_t       *my_chassis[4];
motion_mode_t   motion_mode = NORMAL;

void motion_task_create(void) {
    gimbal_init(&my_gimbal);
    chassis_init(my_chassis);
    osThreadDef(chassisTask, chassis_task, osPriorityAboveNormal, 0, 256);
    chassis_task_handle = osThreadCreate(osThread(chassisTask), NULL);
    if (chassis_task_handle == NULL)
        print("Chassis task create failed.\r\n");
    else
        print("Chassis task created.\r\n");
    osThreadDef(gimbalTask, gimbal_task, osPriorityAboveNormal, 0, 256);
    gimbal_task_handle = osThreadCreate(osThread(gimbalTask), NULL);
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
        yaw_astray = cur_yaw_feedback - INIT_MIDDLE_YAW; // how far
        yaw_astray_in_rad = yaw_astray * MOTOR_2_RAD;
        // TODO: replace the following three 0s with yaw_astray_in_rad
#ifdef USE_REMOTE
        calc_remote_move(my_chassis, rc, yaw_astray_in_rad);
        evasion_mode = (rc->swl == RC_SWITCH_UP);
#else
        calc_keyboard_move(my_chassis, rc, yaw_astray_in_rad);
        evasion_mode = rc->key.bit.X;
#endif
#ifndef ENGINEERING
        if (evasion_mode)
#else
        if (evasion_mode && motion_mode == HORIZONTAL)
#endif
            evasive_move(my_chassis, cur_yaw_feedback, my_gimbal.yaw->motor);
        else
            adjust_chassis_gimbal_pos(my_chassis, my_gimbal.yaw_middle, my_gimbal.yaw->motor);

        run_chassis(my_chassis);
        osDelayUntil(&chassis_wake_time, MOTION_CYCLE);
    }
}

static void engineering_mode_switch(dbus_t *rc) {
    if (motion_mode != HORIZONTAL && rc->swr == RC_SWITCH_MI) {
        chassis_stop();
        motion_mode = HORIZONTAL;
        get_motor_data(my_gimbal.pitch->motor);
        yaw_ramp_ctl(&my_gimbal, get_angle_err(my_gimbal.yaw->motor, 
                    HORIZONTAL_MIDDLE_YAW), 100);
        my_gimbal.yaw_middle = HORIZONTAL_MIDDLE_YAW;
        osDelay(200);
        chassis_mode_forward();
    }
    else if (motion_mode == HORIZONTAL && rc->swr != RC_SWITCH_MI) {
        chassis_stop();
        get_motor_data(my_gimbal.pitch->motor);
        if (rc->swr == RC_SWITCH_UP) {
            motion_mode = NORMAL;
            yaw_ramp_ctl(&my_gimbal, get_angle_err(my_gimbal.yaw->motor, 
                        NORMAL_MIDDLE_YAW), 100);
            my_gimbal.yaw_middle = NORMAL_MIDDLE_YAW;
        }
        else {
            motion_mode = REVERSE;
            yaw_ramp_ctl(&my_gimbal, get_angle_err(my_gimbal.yaw->motor, 
                        REVERSE_MIDDLE_YAW), 100);
            my_gimbal.yaw_middle = REVERSE_MIDDLE_YAW;
        }
        chassis_mode_forward();
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

#ifdef ENGINEERING
        engineering_mode_switch(rc);
        if (rc->swl == RC_SWITCH_UP) 
            HAL_GPIO_WritePin(MAGNET_GPIO_Port, MAGNET_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(MAGNET_GPIO_Port, MAGNET_Pin, GPIO_PIN_RESET);
        
#endif

        observed_absolute_gimbal_yaw = (int32_t)(imuBoard.angle[YAW] * DEG_2_MOTOR);
#ifdef USE_REMOTE
        gimbal_remote_move(&my_gimbal, rc, observed_absolute_gimbal_yaw);
#else
        gimbal_mouse_move(&my_gimbal, rc, observed_absolute_gimbal_yaw);
#endif
        run_gimbal(&my_gimbal);
        osDelayUntil(&gimbal_wake_time, MOTION_CYCLE);
    }
}
