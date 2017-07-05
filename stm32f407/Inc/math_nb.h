#ifndef __DELAY_H
#define __DELAY_H 			   

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdint.h>
#include <math.h>
/**
  * @}
  */

/**
  * @}
  */

typedef struct stdev_s
{
    float m_oldM, m_newM, m_oldS, m_newS;
    int m_n;
} stdev_t;


typedef enum {
    FILTER_LPF,
    FILTER_NOTCH
} biquadFilterType_e;

typedef struct biquadFilter_s {
    float b0, b1, b2, a1, a2;
    float d1, d2;
} biquadFilter_t;

void devClear(stdev_t *dev);
void devPush(stdev_t *dev, float x);
float devVariance(stdev_t *dev);
float devStandardDeviation(stdev_t *dev);


void biquadFilterInitLPF(biquadFilter_t *filter, float filterFreq, uint32_t refreshRate);
void biquadFilterInit(biquadFilter_t *filter, float filterFreq, uint32_t refreshRate, float Q, biquadFilterType_e filterType);
float biquadFilterApply(biquadFilter_t *filter, float input);
float Math_fConstrain(float value, float min, float max);
int16_t Math_Constrain(int16_t value,int16_t min,int16_t max);
float Math_fDeadZone(float value, float min, float max);
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


#endif





























