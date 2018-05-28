#include "test_tx2.h"
#include "data_process.h"
#include "task_tx2.h"
#include "bsp_key.h"

void test_tx2(void) {
    while (1) {
        tx2_info.aim_request.aim_mode = AUTOAIM;
        tx2_packer(&tx2_info, tx2_process, CMD_AIM_REQUEST);
    }
}
