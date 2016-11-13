#ifndef _ACCELEROMETER_H
#define _ACCELEROMETER_H
#include "stm32f10x.h"
/********************加速度设备地址**********************/
#define ACC_ADDR						0x30
/********************加速度内部寄存器********************/
//寄存器地址
#define CTRL_REG1_A 				0x20
#define CTRL_REG2_A					0x21
#define CTRL_REG3_A					0x22
#define CTRL_REG4_A					0x23
#define CTRL_REG5_A					0x24
#define HP_FILTER_RESET_A		0x25
#define REFERENCE_A					0x26
#define STATUS_REG_A				0x27

#define OUT_X_L_A						0x28
#define OUT_X_H_A						0x29
#define OUT_Y_L_A						0x2A
#define OUT_Y_H_A						0x2B
#define OUT_Z_L_A						0x2C
#define OUT_Z_H_A						0x2D

#define INT1_CFG_A					0x30		/*Interrupt 1 config*/
#define INT1_SOURCE_A				0x31		/*Interrupt 1 source*/
#define INT1_THS_A					0x32		/*Interrupt 1 threshold*/
#define INT1_DURATION_A			0x33		/*Interrupt 1 duration*/

#define INT2_CFG_A					0x34		/*Interrupt 2 config*/
#define INT2_SOURCE_A				0x35		/*Interrupt 2 source*/
#define INT2_THS_A					0x36		/*Interrupt 2 threshold*/
#define INT2_DURATION_A			0x37		/*Interrupt 2 duration*/

//函数声明
void Acc_Init(void);
void Acc_Read(s16 Acc_Data[3]);

#endif








