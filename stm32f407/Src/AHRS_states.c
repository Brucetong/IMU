/* ------------------------------------------------------------------------------
  File: UM6_states.c
  Author: CH Robotics
  Version: 1.0
  
  Description: Function definitions for CHR-6dm state estimation.
------------------------------------------------------------------------------ */ 

#include <math.h>
#include "AHRS_states.h"
#include "AHRS_matrix.h"
#include "AHRS_quat.h"
#include "mpu6500.h"
//#include "core_cm3.h"

// Data structures for holding sensor data and estimated states.
extern float quad_to_send[4];
SensorData gSensorData = {0};
AHRS_state_data gStateData = {0};
extern float interval_us;
/*******************************************************************************
* Function Name  : EKF_Init
* Input          : None
* Output         : Pre-initialized state estimate structure
* Return         : None
* Description    : Fills an AHRS_states structure with zero initial values.
*******************************************************************************/
void EKF_Init( void )
{	
	mat_zero( &gStateData.R, 4, 4 );
	mat_zero( &gStateData.Sigma, 4, 4 );

	gStateData.process_var = 50.0;
	gStateData.accel_var = 2.0;
	gStateData.mag_var = 2.0;
	
	// Process variance is scaled here so that the performance in Euler Angle mode and Quaternion mode is comparable
	gStateData.Sigma.data[0][0] = gStateData.process_var*0.00001;
	gStateData.Sigma.data[1][1] = gStateData.process_var*0.00001;
	gStateData.Sigma.data[2][2] = gStateData.process_var*0.00001;
	gStateData.Sigma.data[3][3] = gStateData.process_var*0.00001;
		
	gStateData.R.data[0][0] = gStateData.process_var*0.00001;
	gStateData.R.data[1][1] = gStateData.process_var*0.00001;
	gStateData.R.data[2][2] = gStateData.process_var*0.00001;
	gStateData.R.data[3][3] = gStateData.process_var*0.00001;

	gStateData.qib.a = 1;
	gStateData.qib.b = 0;
	gStateData.qib.c = 0;
	gStateData.qib.d = 0;
}


/*******************************************************************************
* Function Name  : EKF_EstimateStates
* Input          : AHRS_states* estimated_states, RawSensorData* sensor_data
* Output         : None
* Return         : None
* Description    : 
*******************************************************************************/
void EKF_EstimateStates( AHRS_state_data* state_data, SensorData* sensor_data )
{	 
	 // Run EKF prediction step
	 EKF_Predict( state_data, sensor_data );
	 	 
	 // Run EKF update step
	 EKF_Update( state_data, sensor_data );
}



/*******************************************************************************
* Function Name  : EKF_Predict
* Input          : AHRS_states* estimated_states, RawSensorData* sensor_data
* Output         : None
* Return         : None
* Description    : EKF prediction step.  Uses rate gyros to make new orientation
						 estimate.
*******************************************************************************/
void EKF_Predict( AHRS_state_data* estimated_states, SensorData* sensor_data )
{
	float T,p,q,r;	
	quat pqr_quat;
	quat temp_quat;
	fMatrix A,Atranspose,temp1,temp2;
	
	T = 0.001;//interval_us * 0.000001;
	// Copy body frame angular rates to local variables for convenience
	p = (gSensorData.gyro_zero_norm[1] - gStateData.gyro_y);
	q = (gStateData.gyro_x - gSensorData.gyro_zero_norm[0]);
	r = (gStateData.gyro_z - gSensorData.gyro_zero_norm[2]);	
	
	// Quaternion estimation	
	A.rows = 4;
	A.columns = 4;
	Atranspose.rows = 4;
	Atranspose.columns = 4;
	temp1.rows = 4;
	temp1.columns = 4;
	temp2.rows = 4;
	temp2.columns = 4;
		  	  
	// Convert p, q, and r to rad/s
	p = p*3.14159/180;
	q = q*3.14159/180;
	r = r*3.14159/180;

	
	// Create a quaternion to represent rotation rate	
	pqr_quat.a = 0;
	pqr_quat.b = p;
	pqr_quat.c = q;
	pqr_quat.d = r;

	// Predict new quaternion state based on gyro data
	
	quat_mult( &gStateData.qib, &pqr_quat, &temp_quat );
	quat_scalar_mult( &temp_quat, 0.5*T, &temp_quat );
	quat_add( &gStateData.qib, &temp_quat, &gStateData.qib );
	
	// Normalize new predicted state
	quat_norm( &gStateData.qib );
	
	// PROPAGATE COVARIANCE
	// Compute linearized state transition matrix  //�������Ե�״̬ת������  	A��״̬ת�����
	/*																�///	R�ǹ�����������
		[       1, -(T*p)/2, -(T*q)/2, -(T*r)/2]					��//	Sigma��Э����
		[ (T*p)/2,        1,  (T*r)/2, -(T*q)/2]
		[ (T*q)/2, -(T*r)/2,        1,  (T*p)/2]
		[ (T*r)/2,  (T*q)/2, -(T*p)/2,        1]
	*/
	A.data[0][0] = 1;
	A.data[0][1] = -(T*p)/2;
	A.data[0][2] = -(T*q)/2;
	A.data[0][3] = -(T*r)/2;
	
	A.data[1][0] = (T*p)/2;
	A.data[1][1] = 1;
	A.data[1][2] = (T*r)/2;
	A.data[1][3] = -(T*q)/2;
	
	A.data[2][0] = (T*q)/2;
	A.data[2][1] = -(T*r)/2;
	A.data[2][2] = 1;
	A.data[2][3] = (T*p)/2;
	
	A.data[3][0] = (T*r)/2;
	A.data[3][1] = (T*q)/2;
	A.data[3][2] = -(T*p)/2;
	A.data[3][3] = 1;
	
	// Compute the new covariance estimate (discrete update: Sigma = A*Sigma*Atranspose + R
	mat_transpose( &A, &Atranspose );
	mat_mult( &A, &gStateData.Sigma, &temp1 );
	mat_mult( &temp1, &Atranspose, &temp2 );
	mat_add( &temp2, &gStateData.R, &gStateData.Sigma );	 
}

/*******************************************************************************
* Function Name  : EKF_Update
* Input          : AHRS_states* estimated_states, RawSensorData* sensor_data
* Output         : None
* Return         : None
* Description    : EKF update step.  Uses accels to correct pitch and roll errors,
						 and magnetic sensors to correct yaw errors.  Compensation is
						 only applied when new data is available, as specified by the
						 new_mag_data and new_accel_data flags in the sensor_data structure.
*******************************************************************************/
void EKF_Update( AHRS_state_data* estimated_states, SensorData* sensor_data )
{
	 // QUATERNION CODE
	 {
		  float a,b,c,d;
		  
		  // Do accelerometer update if enabled and if there is new accelerometer sensor data available
		  if( gSensorData.new_accel_data )//&& (gConfig.r[UM6_MISC_CONFIG] & UM6_ACCEL_UPDATE_ENABLED) )
		  {
				float sensor_norm = sqrt(gStateData.accel_x*gStateData.accel_x + gStateData.accel_y*gStateData.accel_y + gStateData.accel_z*gStateData.accel_z);
				fMatrix C;
				
				C.rows = 1;
				C.columns = 4;
								
				// Make sure this accelerometer measurement is close to 1 G.  If not, ignore it.
				if( (sensor_norm - 1) < 0.1 && (sensor_norm - 1) > -0.1 )
				{
					 float ax_hat, ay_hat, az_hat;
					 float ax_ref, ay_ref, az_ref;
					 
					 // Make local copies of the current quaternion state estimate for convenience
					 a = gStateData.qib.a;
					 b = gStateData.qib.b;
					 c = gStateData.qib.c;
					 d = gStateData.qib.d;
					 
					 // Make local copy of accelerometer reference vector for convenience
					 ax_ref = gStateData.accel_ref_x;
					 ay_ref = gStateData.accel_ref_y;
					 az_ref = gStateData.accel_ref_z;

					 // Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
					 ax_hat = ay_ref*(2*a*d + 2*b*c) - az_ref*(2*a*c - 2*b*d) + ax_ref*(a*a + b*b - c*c - d*d);
					 					 
					 // Create linearized update matrix for x-axis accelerometer
					 /* For all axes, C is given by:
					 [ 2*a*ax - 2*az*c + 2*ay*d, 2*ax*b + 2*ay*c + 2*az*d, 2*ay*b - 2*a*az - 2*ax*c, 2*a*ay + 2*az*b - 2*ax*d]
					 [ 2*a*ay + 2*az*b - 2*ax*d, 2*a*az - 2*ay*b + 2*ax*c, 2*ax*b + 2*ay*c + 2*az*d, 2*az*c - 2*a*ax - 2*ay*d]
					 [ 2*a*az - 2*ay*b + 2*ax*c, 2*ax*d - 2*az*b - 2*a*ay, 2*a*ax - 2*az*c + 2*ay*d, 2*ax*b + 2*ay*c + 2*az*d]					 
					 */
					 C.data[0][0] = a*ax_ref - az_ref*c + ay_ref*d;
					 C.data[0][1] = ax_ref*b + ay_ref*c + az_ref*d;
					 C.data[0][2] = ay_ref*b - a*az_ref - ax_ref*c;
					 C.data[0][3] = a*ay_ref + az_ref*b - ax_ref*d;

					 
					 // Do correction	��õļ��ٶ�ֵ		����ļӼ�	���ٶ�Э����
					 EKF_Correction( &C, gStateData.accel_x, ax_hat, gStateData.accel_var, &gStateData, ACCEL_UPDATE );
					 
					 // REPEAT FOR Y-AXIS
					 // Make local copies of the current quaternion state estimate for convenience					 
					 // Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
					 ay_hat = az_ref*(2*a*b + 2*c*d) - ax_ref*(2*a*d - 2*b*c) + ay_ref*(a*a - b*b + c*c - d*d);
					 
					 C.data[0][0] = a*ay_ref + az_ref*b - ax_ref*d;
					 C.data[0][1] = a*az_ref - ay_ref*b + ax_ref*c;
					 C.data[0][2] = ax_ref*b + ay_ref*c + az_ref*d;
					 C.data[0][3] = az_ref*c - a*ax_ref - ay_ref*d;

					 
					 // Do correction
					 EKF_Correction( &C, gStateData.accel_y, ay_hat, gStateData.accel_var, &gStateData, ACCEL_UPDATE );

					 // REPEAT FOR Z-AXIS
					 // Make local copies of the current quaternion state estimate for convenience					 
					 // Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
					 az_hat = ax_ref*(2*a*c + 2*b*d) - ay_ref*(2*a*b - 2*c*d) + az_ref*(a*a - b*b - c*c + d*d);
					 
					 C.data[0][0] = a*az_ref - ay_ref*b + ax_ref*c;
					 C.data[0][1] = ax_ref*d - az_ref*b - a*ay_ref;
					 C.data[0][2] = a*ax_ref - az_ref*c + ay_ref*d;
					 C.data[0][3] = ax_ref*b + ay_ref*c + az_ref*d;
					 
					 // Do correction
					 EKF_Correction( &C, gStateData.accel_z, az_hat, gStateData.accel_var, &gStateData, ACCEL_UPDATE );

				}
		  }
		  
		  // Do magnetometer update if enabled and if there is new magnetic sensor data available
		  if( gSensorData.new_mag_data )//&& (gConfig.r[UM6_MISC_CONFIG] & UM6_MAG_UPDATE_ENABLED) )
		  {
				fMatrix C;
				float mx_hat, my_hat, mz_hat;
				float mx_ref, my_ref, mz_ref;				
			
				C.rows = 1;
				C.columns = 4;	 	
				
				// Make local copies of the current quaternion state estimate for convenience
				a = gStateData.qib.a;
				b = gStateData.qib.b;
				c = gStateData.qib.c;
				d = gStateData.qib.d;
				
				// Make local copy of accelerometer reference vector for convenience
				mx_ref = gStateData.mag_ref_x;
				my_ref = gStateData.mag_ref_y;
				mz_ref = gStateData.mag_ref_z;
					
				// Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
				mx_hat = my_ref*(2*a*d + 2*b*c) - mz_ref*(2*a*c - 2*b*d) + mx_ref*(a*a + b*b - c*c - d*d);
									
				// Create linearized update matrix for x-axis accelerometer
				/* For all axes, C is given by:
				[ 2*a*ax - 2*az*c + 2*ay*d, 2*ax*b + 2*ay*c + 2*az*d, 2*ay*b - 2*a*az - 2*ax*c, 2*a*ay + 2*az*b - 2*ax*d]
				[ 2*a*ay + 2*az*b - 2*ax*d, 2*a*az - 2*ay*b + 2*ax*c, 2*ax*b + 2*ay*c + 2*az*d, 2*az*c - 2*a*ax - 2*ay*d]
				[ 2*a*az - 2*ay*b + 2*ax*c, 2*ax*d - 2*az*b - 2*a*ay, 2*a*ax - 2*az*c + 2*ay*d, 2*ax*b + 2*ay*c + 2*az*d]					 
				*/
				C.data[0][0] = a*mx_ref - mz_ref*c + my_ref*d;
				C.data[0][1] = mx_ref*b + my_ref*c + mz_ref*d;
				C.data[0][2] = my_ref*b - a*mz_ref - mx_ref*c;
				C.data[0][3] = a*my_ref + mz_ref*b - mx_ref*d;
				
				// Do correction
				EKF_Correction( &C, gStateData.mag_x, mx_hat, gStateData.mag_var, &gStateData, MAG_UPDATE );
				
				// REPEAT FOR Y-AXIS
				// Make local copies of the current quaternion state estimate for convenience				
				// Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
				my_hat = mz_ref*(2*a*b + 2*c*d) - mx_ref*(2*a*d - 2*b*c) + my_ref*(a*a - b*b + c*c - d*d);
				
				C.data[0][0] = a*my_ref + mz_ref*b - mx_ref*d;
				C.data[0][1] = a*mz_ref - my_ref*b + mx_ref*c;
				C.data[0][2] = mx_ref*b + my_ref*c + mz_ref*d;
				C.data[0][3] = mz_ref*c - a*mx_ref - my_ref*d;

				
				// Do correction
				EKF_Correction( &C, gStateData.mag_y, my_hat, gStateData.mag_var, &gStateData, MAG_UPDATE );
				// REPEAT FOR Z-AXIS
				// Make local copies of the current quaternion state estimate for convenience
				// Compute expected accelerometer measurements based on the current attitude quaternion and the accel reference vector
				mz_hat = mx_ref*(2*a*c + 2*b*d) - my_ref*(2*a*b - 2*c*d) + mz_ref*(a*a - b*b - c*c + d*d);
				
				C.data[0][0] = a*mz_ref - my_ref*b + mx_ref*c;
				C.data[0][1] = mx_ref*d - mz_ref*b - a*my_ref;
				C.data[0][2] = a*mx_ref - mz_ref*c + my_ref*d;
				C.data[0][3] = mx_ref*b + my_ref*c + mz_ref*d;
		
				// Do correction
				EKF_Correction( &C, gStateData.mag_z, mz_hat, gStateData.mag_var, &gStateData, MAG_UPDATE );				
		  
		  }
	 }
}


void EKF_Correction( fMatrix* C, float sensor_data, float sensor_hat, float sensor_covariance, AHRS_state_data* estimated_states, int sensorype )
{
	fMatrix Ctranspose,temp1,temp2,L;
	float gain_scale, error;
	
	mat_zero( &temp1, 4, 1 );
	mat_zero( &temp2, 4, 4 );
	
	mat_transpose( C, &Ctranspose );

	// Compute Kalman Gain (L = Sigma*C'*(C*Sigma*C' + Q)^-1 )
	mat_mult(&gStateData.Sigma,&Ctranspose,&temp1);
	mat_mult(C,&gStateData.Sigma,&temp2);
	mat_mult(&temp2,&Ctranspose,&temp2);
	gain_scale = 1/(temp2.data[0][0] + sensor_covariance);
	mat_scalar_mult(gain_scale,&temp1,&L);
	
	// Update state estimates
	error = sensor_data - sensor_hat;
	
	gStateData.qib.a += L.data[0][0]*error;
	gStateData.qib.b += L.data[1][0]*error;
	gStateData.qib.c += L.data[2][0]*error;
	gStateData.qib.d += L.data[3][0]*error;
	
	mat_create_identity( &temp1, 4, 4 );
	
	// Now update the covariance estimate (Sigma = (I - L*C)*Sigma
	mat_mult(&L,C,&temp2);
	mat_scalar_mult(-1,&temp2,&temp2);
	mat_add(&temp1,&temp2,&temp1);
	mat_mult(&temp1,&gStateData.Sigma, &gStateData.Sigma);
}

/*******************************************************************************
* Function Name  : compute_euler_angles
* Input          : AHRS_states* states
* Output         : None
* Return         : None
* Description    : Converts quaternion attitude estimate to euler angles (yaw, pitch, roll)
*******************************************************************************/
void compute_euler_angles( AHRS_state_data* estimated_states )
{
	 float q0, q1, q2, q3;
	 
	 q0 = estimated_states->qib.a;
	 q1 = estimated_states->qib.b;
	 q2 = estimated_states->qib.c;
	 q3 = estimated_states->qib.d;	 

	 estimated_states->phi = atan2(2*(q0*q1 + q2*q3),q3*q3 - q2*q2 - q1*q1 + q0*q0)*180/3.14159;
	 estimated_states->theta = -asin(2*(q1*q3 - q0*q2))*180/3.14159;
	 estimated_states->psi = atan2(2*(q0*q3+q1*q2),q1*q1 + q0*q0 - q3*q3 - q2*q2)*180/3.14159;
}

/*******************************************************************************
* Function Name  : unroll_states
* Input          : AHRS_states* states
* Output         : None
* Return         : None
* Description    : Keeps all angle estimates in the range of -360 to 360 degrees
*******************************************************************************/
void unroll_states( AHRS_state_data* states )
{
	 while( states->phi > 360 )
	 {
		  states->phi -= 360;
	 }
	 while( states->phi < -360 )
	 {
		  states->phi += 360;
	 }
	 
	 while( states->theta > 360 )
	 {
		  states->theta -= 360;
	 }
	 while( states->theta < -360 )
	 {
		  states->theta += 360;
	 }
	 
	 while( states->psi > 360 )
	 {
		  states->psi -= 360;
	 }
	 while( states->psi < -360 )
	 {
		  states->psi += 360;
	 }
	 
}


