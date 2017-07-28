
#include "mpu6500.h"
#include "delay.h"

volatile uint8_t	mpu6500_buf[14];					//spi读取MPU6500后存放数据
uint8_t offset_flag = 0;						//校准模式标志位，为0未进行校准，为1进行校准
uint8_t yaw_drift_flag = 0;

volatile S_INT16_XYZ	MPU6500_Acc_Offset	=	{0,0,0};
volatile S_INT16_XYZ	MPU6500_Gyro_Offset	=	{0,0,0};
volatile S_INT16_XYZ MPU6500_Acc = {0,0,0};
volatile S_INT16_XYZ MPU6500_Gyro = {0,0,0};

volatile int16_t mpu6500_gyro[3] = {0};
volatile int16_t mpu6500_gyro_offset[3] = {0};
volatile int16_t mpu6500_acc[3] = {0};
volatile int16_t mpu6500_acc_offset[3] = {0};

volatile float	mpu6500_tempreature = 0;
volatile int16_t mpu6500_tempreature_temp = 0;
volatile int16_t mpu6500_tempreature_Offset = 0;

int16_t  mpu6500_FIFO[6][11];
int16_t mpu6500_gyro_ADC[3],mpu6500_acc_ADC[3];
uint8_t mpu6500_fifo_index = 0;

static uint16_t calibratingG = 0;

static uint16_t accLpfCutHz = 0;
static uint16_t accSamplingInterval = 1000;

uint16_t acc_1G = 512 * 4;

static uint16_t calibratingA = 0;      // the calibration is done is the main loop. Calibrating decreases at each cycle down to 0, then we enter in a normal mode.
static biquadFilter_t accFilter[XYZ_AXIS_COUNT];

#define  mpu6500_fifo_size 10


/*
 * 函数名：MPU6500_ReadValue
 * 描述  ：读取MPU6500原始数据
 * 输入  ：无
 * 输出  ：无
 */
void MPU6500_ReadValue(void)
{
	uint8_t i;

	MPU6500_CS(0); 																	//使能SPI传输

	SPI1_Read_Write_Byte(ACCEL_XOUT_H|0x80); 				//从加速度计的寄存器开始进行读取陀螺仪和加速度计的值//发送读命令+寄存器号

	for(i	=	0;i	<	14;i++)														//一共读取14字节的数据
	{
		mpu6500_buf[i]	=	SPI1_ReadWrite_Byte(0xff);	//输入0xff,因为slave不识别
	}

	mpu6500_acc[X] = MPU6500_Acc.X = BYTE16(int16_t, mpu6500_buf[0],  mpu6500_buf[1]) - mpu6500_acc_offset[X];
	mpu6500_acc[Y] = MPU6500_Acc.Y = BYTE16(int16_t, mpu6500_buf[2],  mpu6500_buf[3]) - mpu6500_acc_offset[Y];
	mpu6500_acc[Z] = MPU6500_Acc.Z = BYTE16(int16_t, mpu6500_buf[4],  mpu6500_buf[5]);// - mpu6500_acc_offset[Z];
	mpu6500_gyro[X] = MPU6500_Gyro.X = BYTE16(int16_t, mpu6500_buf[8],  mpu6500_buf[9]) - mpu6500_gyro_offset[X];
	mpu6500_gyro[Y] = MPU6500_Gyro.Y = BYTE16(int16_t, mpu6500_buf[10],  mpu6500_buf[11]) - mpu6500_gyro_offset[Y];
	mpu6500_gyro[Z] = MPU6500_Gyro.Z = BYTE16(int16_t, mpu6500_buf[12],  mpu6500_buf[13]) - mpu6500_gyro_offset[Z];

	mpu6500_tempreature_temp	=	BYTE16(int16_t, mpu6500_buf[6],  mpu6500_buf[7]);
	mpu6500_tempreature	=	(float)(18000+((521+mpu6500_tempreature_temp)*100)/34); // 原来分母为340，现在分子*100，即：扩大1000倍；
	mpu6500_tempreature = mpu6500_tempreature/1000;
	if(( -4	<	MPU6500_Gyro.X ) && (MPU6500_Gyro.X < 4) )
	{
		mpu6500_gyro[X] = MPU6500_Gyro.X = 0;
	}
	if(( -4	<	MPU6500_Gyro.Y ) && (MPU6500_Gyro.Y < 4) )
	{
		mpu6500_gyro[Y] = MPU6500_Gyro.Y = 0;
	}
	if(( -4	<	MPU6500_Gyro.Z ) && (MPU6500_Gyro.Z < 4) )
	{
		mpu6500_gyro[Z] = MPU6500_Gyro.Z = 0;
	}

	MPU6500_CS(1);  	    //禁止SPI传输

	gyro_update();
	accUpdate();

	mpu6500_new_value();
	mpu6500_get_val();
}

/*
 * 函数名：MPU6500_Init
 * 描述  ：MPU6500初始化函数
 * 输入  ：无
 * 输出  ：0：初始化失败 1：初始化成功
 */
uint8_t MPU6500_Init(void)
{
	//SPI1_Init();								//MPU6500 IO口和SPI初始化

	if(MPU6500_Read_Reg(WHO_AM_I) == 0x70)			//正确读取到6500的地址
	{
		//delay_ms(100);
		//printf("init6500 id:%x\n", MPU6500_Read_Reg(WHO_AM_I));
		MPU6500_Write_Reg(PWR_MGMT_1,0X80);   		//电源管理,复位MPU6500
		delay_ms(100);
		MPU6500_Write_Reg(SIGNAL_PATH_RESET,0X07);//陀螺仪、加速度计、温度计复位
		delay_ms(100);
		MPU6500_Write_Reg(PWR_MGMT_1,0X01);   		//选择时钟源
		delay_ms(15);
		MPU6500_Write_Reg(PWR_MGMT_2,0X00);   		//使能加速度计和陀螺仪
		delay_ms(100);
		MPU6500_Write_Reg(CONFIG,0X02);						//低通滤波器 0x02 92hz (3.9ms delay) fs=1khz
		delay_ms(15);
		MPU6500_Write_Reg(SMPLRT_DIV,0X00);				//采样率1000/(1+0)=1000HZ
		delay_ms(15);
		MPU6500_Write_Reg(GYRO_CONFIG,0X18);  		//陀螺仪测量范围 0X18 正负2000度
		delay_ms(15);
		MPU6500_Write_Reg(ACCEL_CONFIG,0x00); 		//加速度计测量范围 0X00 正负2g
		delay_ms(15);
		MPU6500_Write_Reg(ACCEL_CONFIG2,0x00);		//加速度计速率1khz 滤波器460hz (1.94ms delay)
		delay_ms(15);
		return 1;
	}
	else return 0;
}

uint8_t MPU6500_readID(void)
{
	uint8_t id;
	MPU6500_CSL();
	id = SPI1_readReg(0x75);
	MPU6500_CSH();
	return id;
}

//��ȡ���� ��ƽ��ֵ
int16_t MPU6500_getAvg(int16_t * buff, int size) {
  int32_t sum = 0;
  int i;
  for(i=0; i<size; i++) {
    sum += buff[i];
  }
  return sum / size;
}

void mpu6500_new_value()
{
	int16_t new;

	//mpu6500_gyro_ADC[0] ���齫�����PID������
	new = (mpu6500_gyro_ADC[0]*3 + (MPU6500_Gyro.X-MPU6500_Gyro_Offset.X))/4;// range: +/- 8192; +/- 20mpu6500_gyro_offset00 deg/sec
	mpu6500_gyro_ADC[0] = Math_Constrain(new,mpu6500_gyro_ADC[0]-800,mpu6500_gyro_ADC[0]+800);//��Χ�޶�
	new = (mpu6500_gyro_ADC[1]*3 + (MPU6500_Gyro.Y-MPU6500_Gyro_Offset.Y))/4;
	mpu6500_gyro_ADC[1] = Math_Constrain(new,mpu6500_gyro_ADC[1]-800,mpu6500_gyro_ADC[1]+800);
	new = (mpu6500_gyro_ADC[2]*3 + (MPU6500_Gyro.Z-MPU6500_Gyro_Offset.Z))/4;
	mpu6500_gyro_ADC[2] = Math_Constrain(new,mpu6500_gyro_ADC[2]-800,mpu6500_gyro_ADC[2]+800);

	mpu6500_acc_ADC[0]  = MPU6500_Acc.X;
	mpu6500_acc_ADC[1]  = MPU6500_Acc.Y;
	mpu6500_acc_ADC[2]  = MPU6500_Acc.Z;

	mpu6500_FIFO[0][mpu6500_fifo_index] = MPU6500_Acc.X;
	mpu6500_FIFO[1][mpu6500_fifo_index] = MPU6500_Acc.Y;
	mpu6500_FIFO[2][mpu6500_fifo_index] = MPU6500_Acc.Z;
	mpu6500_FIFO[3][mpu6500_fifo_index] = MPU6500_Gyro.X;
	mpu6500_FIFO[4][mpu6500_fifo_index] = MPU6500_Gyro.Y;
	mpu6500_FIFO[5][mpu6500_fifo_index] = MPU6500_Gyro.Z;
	mpu6500_fifo_index = (mpu6500_fifo_index + 1) % mpu6500_fifo_size;//ѭ�����и���

	mpu6500_FIFO[0][10] = MPU6500_getAvg(mpu6500_FIFO[0],mpu6500_fifo_size);
	mpu6500_FIFO[1][10] = MPU6500_getAvg(mpu6500_FIFO[1],mpu6500_fifo_size);
	mpu6500_FIFO[2][10] = MPU6500_getAvg(mpu6500_FIFO[2],mpu6500_fifo_size);
	mpu6500_FIFO[3][10] = MPU6500_getAvg(mpu6500_FIFO[3],mpu6500_fifo_size);
	mpu6500_FIFO[4][10] = MPU6500_getAvg(mpu6500_FIFO[4],mpu6500_fifo_size);
	mpu6500_FIFO[5][10] = MPU6500_getAvg(mpu6500_FIFO[5],mpu6500_fifo_size);
}

#define new_weight 0.4f//��Ȩ��
#define old_weight 0.6f//��Ȩ��

void mpu6500_get_val()
{
	static  float lastacc[3]= {0,0,0};
	int i;

    for(i = 0; i<6; i++) {
      if(i < 3) {
    	  mpu6500_FIFO[i][10] = (float) mpu6500_FIFO[i][10] * new_weight +lastacc[i] * old_weight ;
		lastacc[i] = mpu6500_FIFO[i][10];
      }
      else {
    	  mpu6500_FIFO[i][10] = ((float) mpu6500_FIFO[i][10]) / 16.4f; //ת�ɶ�ÿ��
		//�����Ѿ������̸ĳ��� 2000��ÿ��  16.4 ��Ӧ 1��ÿ��
      }
    }
}

bool isGyroCalibrationComplete(void)
{
    return calibratingG == 0;
}

static bool isOnFinalGyroCalibrationCycle(void)
{
    return calibratingG == 1;
}

static uint16_t gyroCalculateCalibratingCycles(void)
{
    return  CALIBRATING_GYRO_CYCLES;
}

static bool isOnFirstGyroCalibrationCycle(void)
{
    return calibratingG == gyroCalculateCalibratingCycles();
}

void gyroSetCalibrationCycles(void)
{
    calibratingG = gyroCalculateCalibratingCycles();
}

void gyro_update()
{
	if (!isGyroCalibrationComplete())
	{
		performGyroCalibration(48);
	}
}
void performGyroCalibration(uint8_t gyroMovementCalibrationThreshold)
{
    static int32_t g[3];
    static stdev_t var[3];

    for (int axis = 0; axis < 3; axis++) {

        // Reset g[axis] at start of calibration
        if (isOnFirstGyroCalibrationCycle()) {
            g[axis] = 0;
            devClear(&var[axis]);
        }

        // Sum up CALIBRATING_GYRO_CYCLES readings
        g[axis] += mpu6500_gyro[axis];
        devPush(&var[axis], mpu6500_gyro[axis]);

        // Reset global variables to prevent other code from using un-calibrated data
        mpu6500_gyro[axis] = 0;
        mpu6500_gyro_offset[axis] = 0;

        if (isOnFinalGyroCalibrationCycle()) {
            const float dev = devStandardDeviation(&var[axis]);

            // check deviation and startover in case the model was moved
            if (gyroMovementCalibrationThreshold && dev > gyroMovementCalibrationThreshold) {
                gyroSetCalibrationCycles();
                return;
            }
            mpu6500_gyro_offset[axis] = (g[axis] + (gyroCalculateCalibratingCycles() / 2)) / gyroCalculateCalibratingCycles();
        }
    }

    calibratingG--;
}

void setAccelerationFilter(uint16_t initialAccLpfCutHz)
{
    accLpfCutHz = initialAccLpfCutHz;
    if (accSamplingInterval) {
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
            biquadFilterInitLPF(&accFilter[axis], accLpfCutHz, accSamplingInterval);
        }
    }

    //biquadFilterInitLPF(&yawFilter, accLpfCutHz, accSamplingInterval);

    accSetCalibrationCycles(CALIBRATING_ACC_CYCLES);
}

void accUpdate()
{
    if (accLpfCutHz) {
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
        	mpu6500_acc[axis] = lrintf(biquadFilterApply(&accFilter[axis], (float)mpu6500_acc[axis]));
        }
    }

    if (!isAccelerationCalibrationComplete()) {
        performAcclerationCalibration(48);
    }
}


void accSetCalibrationCycles(uint16_t calibrationCyclesRequired)
{
    calibratingA = calibrationCyclesRequired;
}

bool isAccelerationCalibrationComplete(void)
{
    return calibratingA == 0;
}

static bool isOnFinalAccelerationCalibrationCycle(void)
{
    return calibratingA == 1;
}

static bool isOnFirstAccelerationCalibrationCycle(void)
{
    return calibratingA == CALIBRATING_ACC_CYCLES;
}

void performAcclerationCalibration(uint8_t accMovementCalibrationThreshold)
{
    static int32_t a[3];
    static stdev_t var[3];

    for (int axis = 0; axis < 3; axis++) {

        // Reset a[axis] at start of calibration
        if (isOnFirstAccelerationCalibrationCycle())
        {
            a[axis] = 0;
            devClear(&var[axis]);
        }

        // Sum up CALIBRATING_ACC_CYCLES readings
        a[axis] += mpu6500_acc[axis];
        devPush(&var[axis], mpu6500_acc[axis]);

        // Reset global variables to prevent other code from using un-calibrated data
        mpu6500_acc[axis] = 0;
        mpu6500_acc_offset[axis] = 0;

		if (isOnFinalAccelerationCalibrationCycle()) {
            const float dev = devStandardDeviation(&var[axis]);

            // check deviation and startover in case the model was moved
            if (accMovementCalibrationThreshold && dev > accMovementCalibrationThreshold) {
            	accSetCalibrationCycles(CALIBRATING_ACC_CYCLES);
                return;
            }

			// Calculate average, shift Z down by acc_1G and store values in EEPROM at end of calibration
			mpu6500_acc_offset[X] = (a[X] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
			mpu6500_acc_offset[Y] = (a[Y] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;
			mpu6500_acc_offset[Z] = (a[Z] + (CALIBRATING_ACC_CYCLES / 2)) / CALIBRATING_ACC_CYCLES;// - acc_1G;
			//printf("x:%d,y:%d,z:%d dev:%f\n", mpu6500_acc_offset[X], mpu6500_acc_offset[Y], mpu6500_acc_offset[Z], dev);
			gStateData.accel_ref_x = mpu6500_acc_offset[X] * 0.00006103515625L;
			gStateData.accel_ref_y = mpu6500_acc_offset[Y] * 0.00006103515625L;
			gStateData.accel_ref_z = mpu6500_acc_offset[Z] * 0.00006103515625L;

			gSensorData.acc_zero_norm = sqrt(gStateData.accel_ref_x * gStateData.accel_ref_x + gStateData.accel_ref_y * gStateData.accel_ref_y + gStateData.accel_ref_z * gStateData.accel_ref_z);
			gStateData.accel_ref_x /= gSensorData.acc_zero_norm;
			gStateData.accel_ref_y /= gSensorData.acc_zero_norm;
			gStateData.accel_ref_z /= gSensorData.acc_zero_norm;
		}
	}
    calibratingA--;
}

/************************ (C) COPYRIGHT  *****END OF FILE****/
