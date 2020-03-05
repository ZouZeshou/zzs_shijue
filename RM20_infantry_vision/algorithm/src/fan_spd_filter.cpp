
#include "algorithm/include/fan_spd_filter.h"
/*
 * @brief
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = 1;
 *     H = 1;
 *   and @q,@r are valued after prior tests.
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs
 *   state - Klaman filter structure
 *   init_x - initial x state value
 *   init_p - initial estimated error convariance
 * @outputs
 * @retval
 */
void fan_filter_init(kalman1_state *state, float init_x, float init_p, float int_q, float init_r)
{
    state->x = init_x;
    state->p = init_p;
    state->A = 1;
    state->H = 1;
    state->q = int_q;//10e-6;  /* predict noise convariance */
    state->r = init_r;//10e-5;  /* measure error convariance */
}

/*
 * @brief
 *   1 Dimension Kalman filter
 * @inputs
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs
 * @retval
 *   Estimated result
 */
float fan_filter_update(kalman1_state *state, float z_measure ,float sec_now,uint8_t fps)
{
    /* Predict */
    state->x = state->A;
    state->p = state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    state->gain = state->p / (state->p + state->r);
    state->x = state->x + state->gain * (z_measure -  state->x);
    state->p = (1 - state->gain) * state->p;
    /* add some message for get better curve*/
    float t0 = (sec_now ) * 3.33f/16.0f + 1/static_cast<float>(fps);
    state->A = (0.785f*sinf(1.884f*t0)+1.305f)*RAD_TO_ANGLE;
    return state->x;
}
