#ifndef _UTILS_H_
#define _UTILS_H_

#define sign(x) x < 0 ? -1 : 1

#define min(x, y) x < y ? x : y
#define max(x, y) x > y ? x : y

inline int32_t abs_limit(int32_t *data, int32_t lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

inline float fabs_limit(float *data, float lim) {
    if (*data > lim)
        *data = lim;
    else if (*data < -lim)
        *data = -lim;
    return *data;
}

#endif
