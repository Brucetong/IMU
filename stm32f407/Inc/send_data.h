#ifndef __SEND_DATA_H
#define __SEND_DATA_H

#include "stm32f4xx_hal.h"
#include "main.h"

#include "usart.h"
#include "string.h"
#include "IMU.h"
/**
  * @}
  */

/**
  * @}
  */

#define SOF 0xCF
#define SEND_BUF_SIZE 64
#define FLOAT_DATA_SIZE 4

typedef union {
	float  value;
	unsigned char byte[4];
} f_bytes;

//ÕûÊý ÁªºÏÌå
typedef union {
	int16_t  value;
	unsigned char byte[2];
} i_bytes;

typedef union
{
	double value;
	unsigned char byte[8];
} d_bytes;

void send_data_to_pc(uint8_t* buf, uint8_t len);
void send_imu_data_to_pc(void);


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#endif
