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

#include "bsp_tof.h"
#include "FreeRTOS.h"
#include "i2c.h"
#include "cmsis_os.h"

static VL53L0X_Error bsp_vl53l0x_calib(bsp_vl53l0x_t *tof) {
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    if (status == VL53L0X_ERROR_NONE)
        status = VL53L0X_PerformRefCalibration(tof->device,
                            &VhvSettings, &PhaseCal);
    if (status == VL53L0X_ERROR_NONE)
        status = VL53L0X_PerformRefSpadManagement(tof->device,
                            &refSpadCount, &isApertureSpads);
    
    return status;
}

bsp_vl53l0x_t* bsp_vl53l0x_init(uint8_t addr, GPIO_TypeDef *gport, uint16_t gpin, uint32_t ranging_time) {
    bsp_vl53l0x_t *new_tof  = pvPortMalloc(sizeof(bsp_vl53l0x_t));
    new_tof->device         = pvPortMalloc(sizeof(VL53L0X_Dev_t));
    
    new_tof->device->comms_speed_khz    = hi2c2.Init.ClockSpeed / 1000;
    new_tof->device->comms_type         = I2C;
    new_tof->gport                      = gport;
    new_tof->gpin                       = gpin;
    new_tof->ranging_time               = ranging_time;
    new_tof->device->I2cDevAddr         = (addr / 2) * 2; // make sure to be even

    if (!reset_vl53l0x(new_tof))
        return NULL;
    return new_tof;
}

uint16_t get_vl53l0x_dist_milli(bsp_vl53l0x_t *tof) {
    VL53L0X_RangingMeasurementData_t mdata;
    VL53L0X_Error status;

    status = VL53L0X_GetRangingMeasurementData(tof->device, &mdata);
    return mdata.RangeMilliMeter;
}

uint8_t reset_vl53l0x(bsp_vl53l0x_t *tof) {
    VL53L0X_Error status;
    HAL_GPIO_WritePin(tof->gport, tof->gpin, GPIO_PIN_RESET);
    osDelay(50);
    HAL_GPIO_WritePin(tof->gport, tof->gpin, GPIO_PIN_SET);
    osDelay(MAX_BOOTUP_TIME);

    uint8_t addr = tof->device->I2cDevAddr;
    tof->device->I2cDevAddr = VL53L0X_DEFAULT_ADDR;

    status = VL53L0X_DataInit(tof->device);
    status = VL53L0X_StaticInit(tof->device);
    status = bsp_vl53l0x_calib(tof);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(tof->device,
            tof->ranging_time);
    status = VL53L0X_SetDeviceMode(tof->device,
            VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    status = VL53L0X_StartMeasurement(tof->device);

    status = VL53L0X_SetDeviceAddress(tof->device, addr);
    tof->device->I2cDevAddr = addr;

    if (status != VL53L0X_ERROR_NONE)
        return 0;
    return 1;
}
