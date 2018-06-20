#include "test_bsp_power.h"
#include "cmsis_os.h"

void test_bsp_power(void) {
    power_module_init(10, 18, -0.14);
    while (1) {
        print("voltage: %.3f current %.3f\r\n",
                get_volt(), get_current());
        osDelay(50);
    }
}
