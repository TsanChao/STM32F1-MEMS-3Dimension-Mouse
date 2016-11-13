#ifndef _MAGNETOMETER_H
#define _MAGNETOMETER_H

/********************罗盘设备地址**********************/
#define MAG_ADDR						0x3C
/********************罗盘内部寄存器********************/
//寄存器地址
#define CRA_REG_M						0x00		/* Configuration register A */	
#define CRB_REG_M						0x01		/* Configuration register B */
#define MR_REG_M						0x02		/* Mode Register */

#define OUT_X_H_M						0x03
#define OUT_X_L_M						0x04
#define OUT_Y_H_M						0x05
#define OUT_Y_L_M						0x06
#define OUT_Z_H_M						0x07
#define OUT_Z_L_M						0x08

#define SR_REG_Mg						0x09
#define IRA_REG_M		        0x0A
#define IRB_REG_M						0x0B
#define IRC_REG_M						0x0C

//操作模式
/* MR_REG_M (Mode Register) */
#define CC_MODE 						0x00		/* Continuous-conversion mode */
#define SC_MODE							0x01		/* Single-conversion mode */
#define SLEEP_MODE					0x03		/* Sleep mode */

//函数声明
void Mag_Init(void);
void Mag_Read(s16 Mag_Data[3]);

#endif
