#include "task_shoot.h"
#include "dbus.h"

static osEvent      shoot_event;

osThreadId  shoot_task_handle;

void shoot_task_create(void) {
    osThreadDef(shootTask, shoot_task, osPriorityNormal, 0, 256);
    shoot_task_handle = osThreadCreate(osThread(shootTask), NULL);
    if (shoot_task_handle == NULL)
        bsp_error_handler(__FUNCTION__, __LINE__, "Shoot task failed to create.");
}

void shoot_task(void const *argu) {
    /* initialization*/
    shooter_t *my_shooter = shooter_init(NULL, PWM);
    dbus_t *rc = dbus_get_struct();
    /* task loop */
    while (1) {
        if (rc->key.bit.Z)
            flywhl_on(my_shooter);
        if (rc->key.bit.X)
            flywhl_off(my_shooter);
        if (rc->mouse.l && my_shooter->flywhl->state == FLYWHL_ON) {
            /* clear interrupt signal */
            osSignalWait(POKER_STOP_SIGNAL, 0);
            /* activate poker until recieving an interrupt signal */
            do {
                poker_set_speed(my_shooter, -600);
                shoot_event = osSignalWait(POKER_STOP_SIGNAL, 20);
            } while (shoot_event.status != osEventSignal &&
                     !(shoot_event.value.signals & POKER_STOP_SIGNAL));
        }
        else {
            poker_set_speed(my_shooter, 0);
            osDelay(20);
        }
    }
}
