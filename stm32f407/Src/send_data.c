
#include "send_data.h"
#include "AHRS_states.h"

void send_data_to_pc(uint8_t* buf, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		HAL_UART_Transmit(&huart3, buf + i, 1, 1);
	}
}

void send_imu_data_to_pc()
{
	uint8_t buf[SEND_BUF_SIZE] = {0};
	f_bytes data;
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t sum = 0;

	buf[i++] = SOF;
	sum += SOF;

	data.value = gStateData.phi;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	data.value = gStateData.theta;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	data.value = gStateData.psi;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	data.value = gStateData.phi;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	data.value = gStateData.theta;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	data.value = gStateData.psi;

	for (j = 0; j < FLOAT_DATA_SIZE; j++)
	{
		buf[i++] = data.byte[j];
		sum += data.byte[j];
	}

	buf[i++] = sum;

	send_data_to_pc(buf, i);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
