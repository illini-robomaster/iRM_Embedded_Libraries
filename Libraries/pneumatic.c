#include "pneumatic.h"
#include "lib_config.h"

#ifdef HERO

void grabber_grab(void) {
    pneumatic_extend(GRABBER_Port, GRABBER_Pin);
}

void grabber_release(void) {
    pneumatic_reset(GRABBER_Port, GRABBER_Pin);
}

void extend_grabber_mechanics(void) {
    pneumatic_extend(PNEUMATIC_GRAB_EXTEND_Port, PNEUMATIC_GRAB_EXTEND_Pin);
}

void reset_grabber_mechanics(void) {
    pneumatic_reset(PNEUMATIC_GRAB_EXTEND_Port, PNEUMATIC_GRAB_EXTEND_Pin);
}

void front_legs_extend(void) {
    pneumatic_extend(FRONT_LIFT_Port, FRONT_LIFT_Pin);
}

void front_legs_reset(void) {
    pneumatic_reset(FRONT_LIFT_Port, FRONT_LIFT_Pin);
}

void rear_legs_extend(void) {
    pneumatic_extend(REAR_LIFT_Port, REAR_LIFT_Pin);
}

void rear_legs_reset(void) {
    pneumatic_reset(REAR_LIFT_Port, REAR_LIFT_Pin);
}

#endif
