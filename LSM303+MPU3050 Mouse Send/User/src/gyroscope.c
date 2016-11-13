#include "analog_iic.h"
#include "gyroscope.h"
/**********************************************
*函数名称：Gyro_Init
*函数功能：陀螺仪初始化
*入口参数：无
*出口参数：无
**********************************************/
void Gyro_Init(void)
{
   I2C_WriteByte(MPU3050_ADDR,PWR_MGM, 0x80);  		 					//电源管理，使用内部晶振
   I2C_WriteByte(MPU3050_ADDR,SMPLRT_DIV, 0x07);    				//Fsample=1KHZ/(7+1)=125Hz
   //I2C_WriteByte(MPU3050_ADDR,DLPF_FS_SYNC, 0x0E);				//±500°,内部模拟采样频率为1KHz
   I2C_WriteByte(MPU3050_ADDR,DLPF_FS_SYNC, 0x1E);    			//±2000°,内部模拟采样频率1KHz
}
/**********************************************
*函数名称：Gyro_Read
*函数功能：读取陀螺仪数据
*入口参数：s16 Gyro_Data[2]
*出口参数：无
**********************************************/
void Gyro_Read(s16 Gyro_Data[3])									 
{
	unsigned char X_L, X_H, Y_L,Y_H, Z_L, Z_H;
														 
	X_L = I2C_ReadByte(MPU3050_ADDR,GYRO_XOUT_L);
	X_H = I2C_ReadByte(MPU3050_ADDR,GYRO_XOUT_H);
	Y_L = I2C_ReadByte(MPU3050_ADDR,GYRO_YOUT_L);
	Y_H = I2C_ReadByte(MPU3050_ADDR,GYRO_YOUT_H);
	Z_L = I2C_ReadByte(MPU3050_ADDR,GYRO_ZOUT_L);
	Z_H = I2C_ReadByte(MPU3050_ADDR,GYRO_ZOUT_H);

	Gyro_Data[0]=(X_H << 8) + X_L;//陀螺仪测出的X轴原始数据
	Gyro_Data[1]=(Y_H << 8) + Y_L;//陀螺仪测出的Y轴原始数据
	Gyro_Data[2]=(Z_H << 8) + Z_L;//陀螺仪测出的Z轴原始数据
}
