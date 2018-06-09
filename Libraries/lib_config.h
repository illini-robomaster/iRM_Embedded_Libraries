#ifndef LIB_CONFIG_H_
#define LIB_CONFIG_H_

#if defined(INFANTRY1)
    /* gimbal related */
    #define INIT_MIDDLE_YAW         5990 // these data are measured on infantry1.
    #define INIT_MIDDLE_PITCH       6000
    #define EVASIVE_LEFTMOST_YAW    7290
    #define EVASIVE_RIGHTMOST_YAW   4690
    /* chassis related */
    #define MAX_TURN_SPEED          3000
    #define MAX_LINEAR_SPEED        3500
    #define MAX_SPEED               7000
    #define chs_kp                  10
    #define chs_ki                  0.9f
    #define chs_kd                  0
    #define chs_int_lim             200

#elif defined(INFANTRY2)
    #define MEASURED_MIDDLE_YAW     5990
    #define MEASURED_MIDDLE_PITCH   6000

#elif defined(INFANTRY3)
    #define MEASURED_MIDDLE_YAW     5990
    #define MEASURED_MIDDLE_PITCH   6000

#elif defined(ENGINEERING)
    /* gimbal related */
    #define INIT_MIDDLE_YAW         5990 // TODO
    #define INIT_MIDDLE_PITCH       6000 // TODO
    #define NORMAL_MIDDLE_YAW       5990 // TODO
    #define REVERSE_MIDDLE_YAW      2000 // TODO
    #define EVASIVE_LEFTMOST_YAW    0    // no need for evasive mode
    #define EVASIVE_RIGHTMOST_YAW   0
    /* chassis related */
    #define MAX_TURN_SPEED          4000
    #define MAX_LINEAR_SPEED        4500
    #define MAX_SPEED               9000
    #define chs_kp                  10
    #define chs_ki                  0.9f
    #define chs_kd                  0
    #define chs_int_lim             200

#elif defined(HERO)
    #define MEASURED_MIDDLE_YAW     5990
    #define MEASURED_MIDDLE_PITCH   6000
#endif

#define IMU_DT          0.02f    // measurement time interval (in seconds). RTOS should give exact time to me.
#define MOTION_CYCLE    20       // PID cycle (in ms).

#endif
