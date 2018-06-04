#ifndef LIB_CONFIG_H_
#define LIB_CONFIG_H_

#if defined(INFANTRY1)
    #define MEASURED_LEFTMOST_YAW  7387
    #define MEASURED_MIDDLE_YAW    5990 // these data are measured on infantry1.
    #define MEASURED_MIDDLE_PITCH  6000
    #define MEASURED_RIGHTMOST_YAW 4569
#elif defined(INFANTRY2)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(INFANTRY3)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(ENGINEERING)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#elif defined(HERO)
    #define MEASURED_MIDDLE_YAW    5990
    #define MEASURED_MIDDLE_PITCH  6000
#endif

#define IMU_DT          0.02f    // measurement time interval (in seconds). RTOS should give exact time to me.
#define MOTION_CYCLE    20       // PID cycle (in ms).

#endif
