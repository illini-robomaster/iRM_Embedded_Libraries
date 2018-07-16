#include "test_shooter.h"
#include "dbus.h"

void test_shooter(void) {
    shooter_t my_shooter;
    shooter_init(&my_shooter, PWM);

    dbus_t *rc = dbus_get_struct();

    osDelay(4000);

    while (1) {
        if (rc->swl != RC_SWITCH_DN) {
            flywhl_set_output(&my_shooter, 250); 
            if (rc->swl == RC_SWITCH_UP)
                poker_set_speed(&my_shooter, -500);
            else
                poker_set_speed(&my_shooter, 0);
        }
        else {
            flywhl_set_output(&my_shooter, 0);
            poker_set_speed(&my_shooter, 0);
        }
        osDelay(20);
    }
}
