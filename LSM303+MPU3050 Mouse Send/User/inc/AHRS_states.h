/* ------------------------------------------------------------------------------
  File: AHRS_states.c
  Description: Function declarations for CHR-6dm state estimation.
------------------------------------------------------------------------------ */ 

#include "stm32f10x.h"

#ifndef _AHRS_STATES__
#define _AHRS_STATES__


#include "AHRS_matrix.h"
#include "AHRS_quat.h"
// Structure for holding raw sensor data
typedef struct __SensorData {
	 float gyro_data[3];
	 float gyro_zero_norm[3];	 
	 u32 new_gyro_data;
	 
	 float acc_data[3];
	 float acc_zero_norm;
	 u32 new_acc_data;
	 
	 float mag_data[3];
	 float mag_zero_norm;
	 u32 new_mag_data;
} SensorData;


// Structure for storing AHRS states and other data related to state computation
// This structure is, in a way, redundant because all this data is also stored in the
// UM6_config or UM6_data structures.  However, in the config and data strucutres, the 
// data is packaged as UInt32 entries into an array for convenience with communication.
// To use the data as floats, special formatting is required.  This structure provides
// a place to store that data in the expected format, which makes accessing it easier.
typedef struct __AHRS_state_data {
	 
	 // Orientation states
	  
		  float phi;		//ºá¹ö½Ç
		  float theta;	//¸©Ñö½Ç
		  float psi;		//Æ«º½½Ç

//		  float pitch;
//		  float theta;

//		  float roll;
//		  float phi;
/*
	 
	 // Orientation rate states

		  float heading_rate;
		  float yaw_rate;
		  float psi_dot;

		  float pitch_rate;
		  float theta_dot;

		  float roll_rate;
		  float phi_dot;
*/
	 
// Quaternion states "qib" = Quaternion from Inertial to Body
	 quat qib;
	 
	 // Gyro biases
//	 int16 beta_p,beta_q,beta_r;
	 
	 // Accelerometer biases
//	 int16 beta_acc_x, beta_acc_y, beta_acc_z;
	 
	 // Magnetometer biases
//	 int16 beta_mag_x, beta_mag_y, beta_mag_z;
	 
	 // Process noise matrix
	 fMatrix R;
	 
	 // Accelerometer alignment matrix
//	 fMatrix accel_cal;
	 
	 // Gyro alignment matrix
	 fMatrix gyro_cal;
	 
	 // Magnetometer calibration matrix
//	 fMatrix mag_cal;
	 	 
	 // EKF covariance
	 fMatrix Sigma;
	 
	 // Magnetic field reference vector
	 float mag_ref_x;
	 float mag_ref_y;
	 float mag_ref_z;
	 
	 // Accelerometer	reference vector
	 float acc_ref_x;
	 float acc_ref_y;
	 float acc_ref_z;
	 
	 // accelerometer measurement variance
	 float acc_var;
	 
	 // Magnetometer variance
	 float mag_var;
	 
	 // Process variance
	 float process_var;
	 
	 // Entries for storing processed sensor data
	 float gyro_x;
	 float gyro_y;
	 float gyro_z;
	 
	 float acc_x;
	 float acc_y;
	 float acc_z;
	 	 
	 float mag_x;
	 float mag_y;
	 float mag_z;
} AHRS_state_data;

extern SensorData gSensorData;
extern AHRS_state_data gStateData;

extern u8 gEKF_mode;

// Function declarations
void EKF_Init( void );
//void EKF_InitFromSensors( AHRS_state_data* estimated_states, RawSensorData* sensor_data );
void EKF_EstimateStates( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Predict( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Update( AHRS_state_data* estimated_states, SensorData* sensor_data );
void EKF_Correction( fMatrix* C, float sensor_data, float sensor_hat, float sensor_covariance, AHRS_state_data* estimated_states, int sensor_type );
extern void OutputDigit(int);
extern void mat_print(fMatrix* matrix);
//void ConvertRawSensorData( AHRS_state_data* estimated_states, RawSensorData* sensor_data );

#define		MAG_UPDATE			0
#define		ACCEL_UPDATE		1

// EKF "mode" stored in the global variable gEKF_mode
#define		EKF_MODE_QUAT		0
#define		EKF_MODE_EULER		1


void compute_euler_angles( AHRS_state_data* estimated_states );
void compute_qid( AHRS_state_data* estimated_states );
void unroll_states( AHRS_state_data* states );

#endif
