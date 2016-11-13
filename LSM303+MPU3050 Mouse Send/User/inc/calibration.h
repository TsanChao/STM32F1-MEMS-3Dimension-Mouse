/*********************************************************
*文件名称：calibration.h
*功能描述：对calibration.c所用函数声明
**********************************************************/
#ifndef _CALIBRATION_H
#define _CALIBRATION_H
/*******************INCLUDES******************************/
#include "stm32f10x.h"
/*********************************************************/

/*******************DECLARE******************************/
void Sensor_Calibration(void);	//调用三个传感器的校准函数
void Gyro_Calibration(u32 m);			//陀螺仪的校准
void Acc_Calibration(u32 m);				//加速度计的校准
void Mag_Calibration(u32 m);       //罗盘的校准
void Get_New_SensorData(void);				 //获取传感器数据
void Mag_Zero_Test(void);					//罗盘零校准

/********************************************************/

#endif
