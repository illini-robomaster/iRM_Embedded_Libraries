#include "test_referee.h"

void test_referee(void) {
    while (1) {
        print("chassis power: %5.2f ", referee_info.power_heat_data.chassis_power);
        print("chassis current: %5.2f ", referee_info.power_heat_data.chassis_current);
        print("chassis voltage: %5.2f ", referee_info.power_heat_data.chassis_volt);
        print("busrt buffer: %5.2f\r\n", referee_info.power_heat_data.chassis_pwr_buf);
        osDelay(50);
    }
}
