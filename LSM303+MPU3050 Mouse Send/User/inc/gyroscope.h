/**************************************************************
*陀螺仪使用的是GY-83模块，内置陀螺仪模块是MPU3050
*故此文件针对MPU3050的配置
***************************************************************/
#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H
#include "stm32f10x.h"
#define	MPU3050_ADDR  0xD0	      //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
																	//MPU30X0的从设备地址是110100x b，其中x由ADO引脚的电平决定
//定义MPU3050内部地址********************
#define WHO_AM_I	  	0x00
#define PRODUCT_ID  	0x01

#define X_OFFS_USRH 	0x0C
#define X_OFFS_USRL		0x0D
#define Y_OFFS_USRH		0x0E
#define Y_OFFS_USRL   0x0F
#define Z_OFFS_USRH		0x10
#define Z_OFFS_USRL		0x11
#define FIFO_EN				0x12

#define AUX_VDDIO     0x13
#define AUX_SLV_ADDR  0x14
#define	SMPLRT_DIV   	0x15
#define DLPF_FS_SYNC	0x16
#define INT_CFG     	0x17
#define AUX_ADDR      0x18
#define INT_STATUS   	0x1A

#define	TEMP_OUT_H	  0x1B
#define	TEMP_OUT_L	  0x1C

#define	GYRO_XOUT_H	  0x1D
#define	GYRO_XOUT_L		0x1E
#define	GYRO_YOUT_H		0x1F
#define	GYRO_YOUT_L		0x20
#define GYRO_ZOUT_H		0x21
#define GYRO_ZOUT_L		0x22

#define AUX_XOUT_H    0x23
#define AUX_XOUT_L		0x24
#define AUX_YOUT_H    0x25
#define AUX_YOUT_L		0x26
#define AUX_ZOUT_H		0x27
#define AUX_ZOUT_L    0x28

#define FIFO_COUNTH   0x3A
#define FIFO_COUNTL   0x3B
#define FIFO_R        0x3C
#define USER_CTRL     0x3D
#define PWR_MGM	      0x3E
//****************************************
//函数声明
void Gyro_Init(void);
void Gyro_Read(s16 Gyro_Data[3]);

#endif






