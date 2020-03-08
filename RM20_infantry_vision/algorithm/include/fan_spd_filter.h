#ifndef FAN_SPD_FILTER_H
#define FAN_SPD_FILTER_H
#include "main/global.h"
#include "algorithm/include/usr_math.h"
#include "algorithm/include/kalman_filter.hpp"
void fan_filter_init(kalman1_state *state, float init_x, float init_p, float int_q, float init_r);
float fan_filter_update(kalman1_state *state, float z_measure ,float sec_now,uint8_t fps);
#endif // FAN_SPD_FILTER_H
