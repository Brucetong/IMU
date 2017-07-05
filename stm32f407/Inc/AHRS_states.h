/* ------------------------------------------------------------------------------
  File: AHRS_states.c
  Description: Function declarations for CHR-6dm state estimation.
------------------------------------------------------------------------------ */ 

//#include "stm32f10x.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#ifndef _AHRS_STATES__
#define _AHRS_STATES__


#include "AHRS_matrix.h"
#include "AHRS_quat.h"
// Structure for holding raw sensor data
typedef struct __SensorData {
	 float gyro_data[3];
	 float gyro_zero_norm[3];	 
	 uint32_t new_gyro_data;
	 
	 float acc_data[3];
	 float acc_zero_norm;
	 uint32_t new_accel_data;
	 
	 float mag_data[3];
	 float mag_zero_norm;
	 uint32_t new_mag_data;
} SensorData;


// Structure for storing AHRS states and other data related to state computation
// This structure is, in a way, redundant because all this data is also stored in the
// UM6_config or UM6_data structures.  However, in the config and data strucutres, the 
// data is packaged as UInt32 entries into an array for convenience with communication.
// To use the data as floats, special formatting is required.  This structure provides
// a place to store that data in the expected format, which makes accessing it easier.
typedef struct __AHRS_state_data {

	float psi;
	float theta;
	float phi;

	quat qib;		 	// Quaternion states "qib" = Quaternion from Inertial to Body
	fMatrix R;			// Process noise matrix	
	fMatrix Sigma;	    // EKF covariance
	
	// Magnetic field reference vector
	float mag_ref_x;
	float mag_ref_y;
	float mag_ref_z;	
	// Accelerometer	reference vector
	float accel_ref_x;
	float accel_ref_y;
	float accel_ref_z;
	
	
	float accel_var;	// accelerometer measurement variance
	float mag_var;	    // Magnetometer variance
	float process_var;  // Process variance
	
	// Entries for storing processed sensor data
	float gyro_x;
	float gyro_y;
	float gyro_z;
	
	float accel_x;
	float accel_y;
	float accel_z;
	
	float mag_x;
	float mag_y;
	float mag_z;

} AHRS_state_data;

extern SensorData gSensorData;
extern AHRS_state_data gStateData;

extern uint8_t gEKF_mode;

// Function declarations
void EKF_Init( void );
void EKF_EstimateStates( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Predict( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Update( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Correction( fMatrix* C, float sensor_data, float sensor_hat, float sensor_covariance, AHRS_state_data* estimated_states, int sensor_type );

#define		MAG_UPDATE			0
#define		ACCEL_UPDATE		1

// EKF "mode" stored in the global variable gEKF_mode
#define		EKF_MODE_QUAT		0
#define		EKF_MODE_EULER		1


void compute_euler_angles( AHRS_state_data* estimated_states );
void compute_qid( AHRS_state_data* estimated_states );
void unroll_states( AHRS_state_data* states );

#endif
