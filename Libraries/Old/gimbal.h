//
// Created by Ma Yuan on 21/12/2017.
//

#ifndef IRM2018_GIMBAL_H
#define IRM2018_GIMBAL_H

#include "pid.h"
#include "bsp_can.h"
void GimbalPIDSetMode(uint32_t mode);
void GimbalPIDAutoSet(uint8_t target);
void GimbalPIDManSet(int16_t yaw_X, int16_t pit_Y);
void GimbalPIDMouseIMU(int16_t yaw_X, int16_t pit_Y);
static int16_t manYaw, manPit; // cumulating position for man mode
static bool manflag;  // flag to show the first frame changed from auto mode to man mode
                      // be used to refesh manYaw and manPit
static bool mouseflag;
#endif //IRM2018_GIMBAL_H
