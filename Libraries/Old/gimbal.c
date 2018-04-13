/***************************************************
 * purpose    : gimbal function
 * description: toplevel of gimbal control
 * modified   : Jeff Ma 2017/12/21
 **************************************************/
#include "gimbal.h"
/* PID Initialize */

/* motor info
    For M3508 P19 Motor with C620 ESC
        Data in range -16384 ~ 16384
        Current in range -20A ~ 20A
    For 6623 Motor
        Data in range -5000 ~ 5000
*/
// constructor function info
/*
void (*f_param_init)(struct __motor_pid_t *pid,  //PID参数初始化
                     uint32_t pid_mode,
                     uint32_t maxOutput,
                     uint32_t integralLimit,
                     float p,
                     float i,
                     float d);
*/

void GimbalPIDSetMode(uint32_t mode){
    if (mode == GIMBAL_AUTO_SHOOT){
        manflag=1;
        mouseflag=1;

        PID_struct_init(&pid_pit, GIMBAL_AUTO_SHOOT, 4800/2, 900/2, -3.0f, -1.0f, -40.0f, 0.0f);
        PID_struct_init(&pid_yaw, GIMBAL_AUTO_SHOOT, 4800/2, 100/2, -4.0f, -1.0f, -75.0f, 0.0f);
    }
    else if (mode == GIMBAL_MAN_SHOOT){
        PID_struct_init(&pid_pit, GIMBAL_MAN_SHOOT, 4800/2, 900/2, -6.0f, 0.0f, -40.0f, 0.0f);//100/2, 2.0f, 10.0f, 15.0f, 0.0f);
        PID_struct_init(&pid_yaw, GIMBAL_MAN_SHOOT, 4800/2, 10/2, -6.0f, -0.0f, -60.0f, 0.0f);//10/2, 3.5f, 1.0f, 30.0f, 0.0f);
    }
    else if (mode == GIMBAL_MOUSE_IMU_SHOOT){
        PID_struct_init(&pid_pit, GIMBAL_MOUSE_IMU_SHOOT, 4800/2, 900/2, -4.0f, 0.0f, -9.0f, 0.0f);//100/2, 2.0f, 10.0f, 15.0f, 0.0f);
        PID_struct_init(&pid_yaw, GIMBAL_MOUSE_IMU_SHOOT, 4800/2, 10/2, -11.0f, -0.0f, -75.0f, 0.0f);//10/2, 3.5f, 1.0f, 30.0f, 0.0f);
    }
    return;
}

/* target scene (yaw,pitch)
                                        yaw right bounder 2380
            *************************************************   pitch upper limit 6280 (include tolerance)
            *       1       *       2       *       3       *
            *  (5200,6390)  *  (4631,6390)  *  (4060,6390)  *
            *************************************************
            *       4       *       5       *       6       *
            *  (5200,6700)  *  (4631,6700)  *  (4060,6700)  *
            *************************************************
            *       7       *       8       *       9       *
            *  (5200,7090)  *  (4631,7090)  *  (4060,7090)  *
            *************************************************   pitch lower limit 7434 (include tolerance)
            yaw left bounder 6883



            for new gimbal
                                        yaw right bounder 5700
            *************************************************   pitch upper limit 8110 (include tolerance)
            *       1       *       2       *       3       *
            *  (7900,7970)  *  (7200,7970)  *  (6500,7970)  *
            *************************************************
            *       4       *       5       *       6       *
            *  (7900,7800)  *  (7200,7800)  *  (6500,7800)  *
            ****************************** *******************
            *       7       *       8       *       9       *
            *  (7900,7625)  *  (7200,7625)  *  (6500,7625)  *
            *************************************************   pitch lower limit 6830 (include tolerance)
            yaw left bounder 510

        	For 6623,
        		DATA[0]DATA[1] = Mechanical Angle in range 0 ~ 8191 - 0 ~ 360 degree
        		DATA[2]DATA[3] = Actual torque current in range -13000 ~ 13000
        		DATA[4]DATA[5] = Given torque current
        		DATA[6]DATA[7] = NULL

					For infantry gimbal
            *************************************************
            *       1       *       2       *       3       *
            *  (6524,7251)  *  (6524,6057)  *  (6524,4863)  *
            *************************************************
            *       4       *       5       *       6       *
            *  (5597,7251)  *  (5597,6057)  *  (5597,4863)  *
            ****************************** *******************
            *       7       *       8       *       9       *
            *  (4670,7251)  *  (4670,6057)  *  (4670,4863)  *
            *************************************************
*/
void GimbalPIDAutoSet(uint8_t target){
    int set_pit;     // goal angle for pitch
    int set_yaw;     // goal angle for yaw

    if(target==1) {
        set_yaw=6500; //7900;
        set_pit=6300; //7970;
    } else if(target==2) {
        set_yaw=6000;
        set_pit=6300; //7970;
    } else if(target==3) {
        set_yaw=5500; //6500;
        set_pit=6300; //7970;
    } else if(target==4) {
        set_yaw=6500; //7900;
        set_pit=5800; //7800;
    } else if(target==5) {
        set_yaw=6000;
        set_pit=5800; //7800;
    } else if(target==6) {
        set_yaw=5500; //6500;
        set_pit=5800; //7800;
    } else if(target==7) {
        set_yaw=6500; //7900;
        set_pit=5300; //7625;
    } else if(target==8) {
        set_yaw=6000;
        set_pit=5300; //7625;
    } else if(target==9){
        set_yaw=5500; //6500;
        set_pit=5300; //7625;
    } else {  // auto-shooting target
        set_yaw=6000;
        set_pit=5800; //7800;
    }
    pid_calc(&pid_pit, motorPitch.angle, set_pit);
    pid_calc(&pid_yaw, motorYaw.angle, set_yaw);


    //RM_PRINTF("(%u, %u)pitch %d;yaw %d ", motorPitch.angle, motorYaw.angle, (int) pid_pit.pos_out, (int) pid_yaw.pos_out);
    //RM_PRINTF("(%d;%d;", (int16_t) pid_pit.pos_out, (int16_t) pid_yaw.pos_out);
    RM_CAN_SetMotor(&hcan1, CAN_6623_1_4_TX, (int16_t) (pid_yaw.pos_out), (int16_t) (pid_pit.pos_out), 0x000, 0x000);
    //RM_PRINTF("(%u, %u)pitch %d;yaw %d\r\n", motorPitch.angle, motorYaw.angle, (int) pid_pit.pos_out, (int) pid_yaw.pos_out);
    //RM_PRINTF("(%d;%d;\r\n", (int) pid_pit.pos_out, (int) pid_yaw.pos_out);
    //RM_PRINTF("%d;%d;%d;%d;%d;%d;%d;%d;%d\r\n", (int) (motorPitch.angle), (int) (motorYaw.angle), (int)(motorChassis[0].angle), (int)(motorChassis[1].angle), \
     (int)(motorChassis[2].angle), (int)(motorChassis[3].angle), (int)(motorPoke.totalAngle), (int)(motorLauncher[0].speedRPM), (int)(motorLauncher[1].speedRPM));


}
/*
 * for mouse control: yaw_X: [-660, 532] pit_Y: [-660, 644]
 * mouseflag
 */

void GimbalPIDMouseIMU(int16_t yaw_X, int16_t pit_Y){
    /* when set from auto mode to man mode
     * record current value for motorPitch.angle and motorYaw.angle
    */
    if(mouseflag==1){
        manPit = motorPitch.angle+160;
        manYaw = motorYaw.angle;
        manflag=0;
    }
    /* keep cumulating and updating manPit and manYaw
    */
    if(pit_Y>10 || pit_Y<-10){ // ignore noise
        manPit+=pit_Y/30;

        while(manYaw>8191){
            manYaw -= 8191;
        }
        while(manYaw<0) {
            manYaw += 8191;
        }

    }
    if(yaw_X>10 || yaw_X<-10){ // ignore noise
        manYaw-=yaw_X/15;
        while(manYaw>8191){
            manYaw -= 8191;
        }
        while(manYaw<0) {
            manYaw += 8191;
        }

    }
    manYaw=(manYaw+7200)/2;
    /*
     * set bound for manPit
     */
    if(manPit<6830) manPit=6830;
    if(manPit>8110) manPit=8110;
    /*
     * set bound for manYaw
     */
    if(manYaw<5300 && manYaw>991) manYaw=5300;
    if(manYaw>910 && manYaw<=991) manYaw=910;

    pid_calc(&pid_pit, motorPitch.angle, manPit);
    pid_calc(&pid_yaw, motorYaw.angle, manYaw);

    RM_CAN_SetMotor(&hcan1, CAN_6623_1_4_TX, (int16_t) (pid_yaw.pos_out), (int16_t) (pid_pit.pos_out), 0x000, 0x000);
}
/*
 * for remote control: yaw_X: [-660, 532] pit_Y: [-660, 644]
 */
void GimbalPIDManSet(int16_t yaw_X, int16_t pit_Y){

    /* when set from auto mode to man mode
     * record current value for motorPitch.angle and motorYaw.angle
    */

    if (manflag==1) {
        manPit = motorPitch.angle+160;
        manYaw = motorYaw.angle;
        manflag=0;
    }
    /* keep cumulating and updating manPit and manYaw
     */

    if(pit_Y>10 || pit_Y<-10){ // ignore noise
        manPit+=pit_Y/30;

        while(manYaw>8191){
            manYaw -= 8191;
        }
        while(manYaw<0) {
            manYaw += 8191;
        }

    }
    if(yaw_X>10 || yaw_X<-10){ // ignore noise
        manYaw-=yaw_X/15;
        while(manYaw>8191){
            manYaw -= 8191;
        }
        while(manYaw<0) {
            manYaw += 8191;
        }

    }
    //   manYaw=(int)(((float)manYaw-6000.0)/1.01+6000.0);
    /*
     * set bound for manPit
     */
    if(manPit<4800) manPit=4800;
    if(manPit>6800) manPit=6800;
    /*
     * set bound for manYaw
     */
    if(manYaw<7500 && manYaw>6500) manYaw=6500;
    if(manYaw>4500 && manYaw<=5500) manYaw=5500;

    pid_calc(&pid_pit, motorPitch.angle, manPit);
    pid_calc(&pid_yaw, motorYaw.angle, manYaw);

    RM_CAN_SetMotor(&hcan1, CAN_6623_1_4_TX, (int16_t) (pid_yaw.pos_out), (int16_t) (pid_pit.pos_out), 0x000, 0x000);

	}
