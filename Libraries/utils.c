#include "utils.h"

int32_t abs_limit(int32_t *data, int32_t lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

float fabs_limit(float *data, float lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

int32_t clip_to_range(int32_t *data, int32_t low_lim, int32_t high_lim) {
    if (low_lim > high_lim)
        return *data;
    if (*data < low_lim)
        *data = low_lim;
    if (*data > high_lim)
        *data = high_lim;
    return *data;
}

float fclip_to_range(float *data, float low_lim, float high_lim) {
    if (low_lim > high_lim)
        return *data;
    if (*data < low_lim)
        *data = low_lim;
    if (*data > high_lim)
        *data = high_lim;
    return *data;
}
