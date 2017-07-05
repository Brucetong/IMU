#include "IMU.h"
#include "arm_math.h"


// Fast inverse square-root
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   float invSqrt(float x)
*��������:	   ���ټ��� 1/Sqrt(x) 	
��������� Ҫ�����ֵ
��������� ���
*******************************************************************************/
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


void IMU_init(void)
{	 
	while(!MPU6500_Init());

	gyroSetCalibrationCycles();
	setAccelerationFilter(10);
}

// a varient of asin() that checks the input ranges and ensures a
// valid angle as output. If nan is given as input then zero is
// returned.
float safe_asin(float v)
{
	if (isnan(v)) {
		return 0.0f;
	}
	if (v >= 1.0f) {
		return M_PI/2;
	}
	if (v <= -1.0f) {
		return -M_PI/2;
	}
	return asin(v);
}

extern SensorData gSensorData;
extern AHRS_state_data gStateData;

void Get_New_Sensor_Data()
{
	gStateData.gyro_x = (float)mpu6500_FIFO[3][10];//gSensorData.gyro_data[0];
	gStateData.gyro_y = (float)mpu6500_FIFO[4][10];//gSensorData.gyro_data[1];
	gStateData.gyro_z = (float)mpu6500_FIFO[5][10];//gSensorData.gyro_data[2];
	gSensorData.new_gyro_data = 1;

	gStateData.accel_x = (float)mpu6500_FIFO[0][10];//0 - gSensorData.acc_data[1];
	gStateData.accel_y = (float)mpu6500_FIFO[1][10];//gSensorData.acc_data[0];
	gStateData.accel_z = (float)mpu6500_FIFO[2][10];//gSensorData.acc_data[2];
	gSensorData.new_accel_data = 1;
/*
	gStateData.mag_x = 0 - gSensorData.mag_data[1];
	gStateData.mag_y = gSensorData.mag_data[0];
	gStateData.mag_z = gSensorData.mag_data[2];
	gStateData.mag_x /= gSensorData.mag_zero_norm;
	gStateData.mag_y /= gSensorData.mag_zero_norm;
	gStateData.mag_z /= gSensorData.mag_zero_norm;
	gSensorData.new_mag_data = 1;*/
}

void EKF_process()
{
	Get_New_Sensor_Data();

	EKF_EstimateStates( &gStateData, &gSensorData );

	compute_euler_angles(&gStateData);

	unroll_states(&gStateData);

	//freq++;
}
