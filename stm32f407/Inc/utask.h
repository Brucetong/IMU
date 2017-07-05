#ifndef __UTASK_H
#define __UTASK_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include "main.h"
#include "led.h"
#include "pid.h"
#include "gpio.h"
#include "usart.h"
#include "IMU.h"
#include "send_data.h"

typedef struct
{
	uint8_t task_state;
	uint8_t led_state;
	uint8_t imu_state;
}g_status_t;

typedef struct
{
	float angle[3];
	uint16_t temperature;
	float mpu6050_temperature;
}imu_data_t;

typedef struct
{
	imu_data_t imu;
}g_real_t;

typedef void(*task_ptr)(void);

typedef struct
{
	uint8_t freq;
	task_ptr handler;
}task_table_t;


void EKF_process(void);


const static task_table_t fast_task_table[] =
{
	//{1, spi_imu_process},
	{1, EKF_process}
};

void led_control(void);//emituofo
void test_led_table(void);
void led_event_check(void);

const static task_table_t slow_task_table[] =
{
	//{25, led_change_state},
	{1, led_control},
	//{200, test_led_table},
	{1, led_event_check},
	{5, send_imu_data_to_pc},//qt
};

const static task_table_t super_task_table[] =
{
	{1, MPU6500_ReadValue},
};

extern volatile g_status_t g_status;
extern volatile g_real_t g_real;
extern task_table_t *fast_table_ptr;
extern uint8_t fast_table_size;
extern task_table_t *slow_table_ptr;
extern uint8_t slow_table_size ;

void slow_task(void);
void fast_task(void);
void super_task(void);
void register_task_table(task_table_t *fast_task_table, uint8_t fast_size,
		task_table_t *slow_task_table, uint8_t slow_size,
		task_table_t *super_task_table, uint8_t super_size);


/**
  * @}
  */

/**
  * @}
  */
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
