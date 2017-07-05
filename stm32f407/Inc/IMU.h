#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx_hal.h"
#include <math.h>
#include "mpu6500.h"
#include "math_nb.h"
#include "AHRS_states.h"
#include <arm_math.h>

#define M_PI  (float)3.1415926535

extern float invSqrt(float x);

void Get_New_Sensor_Data(void);
void EKF_process(void);
#endif

//------------------End of File----------------------------
