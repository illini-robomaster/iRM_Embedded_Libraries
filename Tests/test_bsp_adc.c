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

#include "test_bsp_adc.h"
#include "cmsis_os.h"

void test_bsp_adc(void) {
    adc_t *adc1 = adc_dma_enable(NULL, &hadc1, 0, 100);
    adc_t *adc2 = adc_dma_enable(NULL, &hadc1, 1, 100);
    while (1) {
        uint16_t chan0 = adc_get_val(adc1);
        uint16_t chan1 = adc_get_val(adc2);
        printf("channel 0: %d channel 1: %d\r\n", chan0, chan1);
        osDelay(300);
    }
}
