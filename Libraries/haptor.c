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

#include "haptor.h"
#include "bsp_print.h"

motor_t         *m_haptor;
pid_ctl_t       *pid_haptor;
haptor_state_t  haptor_state;

void init_haptor() {
    m_haptor = malloc(sizeof(motor_t));
    pid_haptor = malloc(sizeof(pid_ctl_t));
    haptor_state = MIDDLE;
    motor_init(m_haptor, 0x206, CAN1_ID, M2006);
    pid_init(pid_haptor, HAPTOR, m_haptor, -8000, 8000, 700000, 0, 0, 13, 0.04, 0, 1300, 10);
}

void haptor_down() {
    while (haptor_state != BOTTOM) {
        get_motor_data(m_haptor);
        m_haptor->out = pid_speed_ctl_speed(pid_haptor, -8000);
        set_motor_output(NULL, m_haptor, NULL, NULL);
        if (abs(pid_haptor->integrator) == pid_haptor->int_lim)
            haptor_state = BOTTOM;
        /* TODO: the delay should be integrated with thread timer */
        HAL_Delay(5);
    }
    m_haptor->out = 0;
    set_motor_output(NULL, m_haptor, NULL, NULL);
}

void haptor_up() {
    while (haptor_state != TOP) {
        get_motor_data(m_haptor);
        m_haptor->out = pid_speed_ctl_speed(pid_haptor, 8000);
        set_motor_output(NULL, m_haptor, NULL, NULL);
        if (abs(pid_haptor->integrator) == pid_haptor->int_lim)
            haptor_state = TOP;
        HAL_Delay(5);
    }
    m_haptor->out = 0;
    set_motor_output(NULL, m_haptor, NULL, NULL);
}
