/************************************************************
*文件名：calibration.c
*文件描述：将获取的陀螺仪、加速度计、罗盘三者的数据进行初步校准，
*          具体方法参见AN3192
************************************************************/

/******************GLOBAL VARIETIES**************************/

/************************INCLUDES****************************/
#include "stm32f10x.h"
#include "calibration.h"
#include "ahrs_states.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "magnetometer.h"
#include "usart.h"
#include <math.h>
/************************************************************/
/*********************VARIETIES******************************/
extern SensorData gSensorData;  					//原始传感器数据
extern AHRS_state_data gStateData; 					//姿态数据结构体
u8 mag_zero_test_flag = 0;							//罗盘初始归零标志位
s32 s32_x_mid = 0, s32_y_mid = 0, s32_z_mid = 0;	//三个轴应该校准的值
/************************************************************/

/**********************FUNCTIONS*****************************/


/*************************************************************
*函数名称:Sensor_Calibration(void)
*函数功能：对陀螺仪、加速度计、罗盘三者的原始数据校准
*入口参数：无
*出口参数：无
**************************************************************/
void Sensor_Calibration()
{
	Gyro_Calibration(1000);	//陀螺仪的校准函数
	Acc_Calibration(1000);	//加速度计的校准函数
	Mag_Calibration(1000);		//罗盘的校准函数
}

/*************************************************************
*函数名称:Gyro_Calibration(u32 m)
*函数功能：陀螺仪的校准函数
*入口参数：
*出口参数：无
**************************************************************/
void Gyro_Calibration(u32 m)
{
	s16 gyro_avg[3]={0,0,0};
  s16 Gyro_Data[3]={0,0,0};
	u32 n = 0;
	while(1)
	{
		Gyro_Read(Gyro_Data);
		//将传感器获得的数据按照量程转换成实际的角速度值		         
		gSensorData.gyro_data[0] = Gyro_Data[0] * 0.06103515625L;//量程设置的±2000°
    gSensorData.gyro_data[1] = Gyro_Data[1] * 0.06103515625L;//量程设置的±2000°
    gSensorData.gyro_data[2] = Gyro_Data[2] * 0.06103515625L;//量程设置的±2000°		

		gyro_avg[0] += gSensorData.gyro_data[0];
		gyro_avg[1] += gSensorData.gyro_data[1];
		gyro_avg[2] += gSensorData.gyro_data[2];

		
		n++;
		
		if(n==m)
		{
			gSensorData.gyro_zero_norm[0] = gyro_avg[0] / m;
			gSensorData.gyro_zero_norm[1] = gyro_avg[1] / m;
			gSensorData.gyro_zero_norm[2] = gyro_avg[2] / m;
			break;
		}	
	}	
}


/*************************************************************
*函数名称:Acc_Calibration(u32 m)
*函数功能：加速度计的校准函数
*入口参数：m,校准次数
*出口参数：gEstimatedStates.accel_ref_i(i=x,y,z)
**************************************************************/
void Acc_Calibration(u32 m)
{
	float acc_avg[3]={0,0,0};
	s16 Acc_Data[3];
  u32 n=0;
	while(1)
	{
		Acc_Read(Acc_Data);
		//将传感器获得的数据按照量程转换成实际的加速度值
		gSensorData.acc_data[0] = Acc_Data[0] * 0.00006103515625L; /***********0.00006103515625L	=	 1/16384 ***************/
		gSensorData.acc_data[1] = Acc_Data[1] * 0.00006103515625L;
		gSensorData.acc_data[2] = Acc_Data[2] * 0.00006103515625L;
		
		acc_avg[0] += gSensorData.acc_data[0];
		acc_avg[1] += gSensorData.acc_data[1];
		acc_avg[2] += gSensorData.acc_data[2];
		n++;

		if(n==m)
		{
			gStateData.acc_ref_x=(acc_avg[0] / m);
			gStateData.acc_ref_y=(acc_avg[1] / m);
			gStateData.acc_ref_z=(acc_avg[2] / m);

		  gSensorData.acc_zero_norm = sqrt(gStateData.acc_ref_x * gStateData.acc_ref_x + gStateData.acc_ref_y * gStateData.acc_ref_y + gStateData.acc_ref_z * gStateData.acc_ref_z);
			gStateData.acc_ref_x /= gSensorData.acc_zero_norm;
			gStateData.acc_ref_y /= gSensorData.acc_zero_norm;
			gStateData.acc_ref_z /= gSensorData.acc_zero_norm;
			break;
		}			
	}	
}


/*************************************************************
*函数名称:Mag_Calibration(u32 m)
*函数功能：罗盘的校准函数
*入口参数：m,校准次数
*出口参数：gEstimatedStates.mag_ref_i(i=x,y,z)
**************************************************************/
void Mag_Calibration(u32 m)
{
	float mag_avg[3]={0,0,0};
  s16 Mag_Data[3]={0,0,0};
	u32 n=0;
	while(1)
	{
		Mag_Read(Mag_Data);
		mag_avg[0] +=Mag_Data[0];
		mag_avg[1] +=Mag_Data[1];
		mag_avg[2] +=Mag_Data[2];
		n++;
		
		if(n==m)
		{
			gStateData.mag_ref_x=(mag_avg[0] / m);
			gStateData.mag_ref_y=(mag_avg[1] / m);
			gStateData.mag_ref_z=(mag_avg[2] / m);

		  gSensorData.mag_zero_norm = sqrt(gStateData.mag_ref_x * gStateData.mag_ref_x + gStateData.mag_ref_y * gStateData.mag_ref_y + gStateData.mag_ref_z * gStateData.mag_ref_z);
			gStateData.mag_ref_x /= gSensorData.mag_zero_norm;
			gStateData.mag_ref_y /= gSensorData.mag_zero_norm;
			gStateData.mag_ref_z /= gSensorData.mag_zero_norm;
			break;
		}
	}	
}

/*************************************************************
*函数名称:Get_New_SensorData(void)
*函数功能:获取传感器数据
*入口参数：无
*出口参数：无
**************************************************************/
void Get_New_SensorData(void)
{
	s16 Gyro_Data[3],Acc_Data[3],Mag_Data[3];
	extern u32 data_ready;
	Gyro_Read(Gyro_Data);
	Acc_Read(Acc_Data);
	Mag_Read(Mag_Data);
	gSensorData.gyro_data[0]=Gyro_Data[0] * 0.06103515625L;/**2000 / 32768=0.06103515625**/
	gSensorData.gyro_data[1]=Gyro_Data[1] * 0.06103515625L;
	gSensorData.gyro_data[2]=Gyro_Data[2] * 0.06103515625L;
	
	gSensorData.acc_data[0]=Acc_Data[0] * 0.00006103515625L;/**2.0 / 32768**/
	gSensorData.acc_data[1]=Acc_Data[1] * 0.00006103515625L;
	gSensorData.acc_data[2]=Acc_Data[2] * 0.00006103515625L;
	
	gSensorData.mag_data[0]=Mag_Data[0];
	gSensorData.mag_data[1]=Mag_Data[1];
	gSensorData.mag_data[2]=Mag_Data[2];
	
	/* 在这里将载体坐标系与参考坐标系的方向调整对应*/
	gStateData.gyro_x =   gSensorData.gyro_data[1];   
	gStateData.gyro_y = - gSensorData.gyro_data[0];
	gStateData.gyro_z =   gSensorData.gyro_data[2];
	gSensorData.new_gyro_data = 1;
	
	gStateData.acc_x = gSensorData.acc_data[0];
	gStateData.acc_y = gSensorData.acc_data[1];
	gStateData.acc_z = gSensorData.acc_data[2];
	gSensorData.new_acc_data = 1;
	
	gStateData.mag_x = gSensorData.mag_data[0];
	gStateData.mag_y = gSensorData.mag_data[1];
	gStateData.mag_z = gSensorData.mag_data[2];
	
	gStateData.mag_x /= gSensorData.mag_zero_norm;
	gStateData.mag_y /= gSensorData.mag_zero_norm;
	gStateData.mag_z /= gSensorData.mag_zero_norm;
	gSensorData.new_mag_data = 1;
	
	data_ready = 1;
}

/*************************************************************
*函数名称:Mag_Zero_Test(void)
*函数功能:罗盘零检测
*入口参数：无
*出口参数：无
**************************************************************/
void Mag_Zero_Test(void)
{
	s16 Mag_Data[3];
	s32 s32_x_max=0,s32_x_min=0,s32_y_max=0,s32_y_min=0,s32_z_max=0,s32_z_min=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//浮空输入-RX
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_1);

	mag_zero_test_flag = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
	//如果flash里面没有罗盘的校准数据的话，初始化为0
	FLASH_SetLatency(FLASH_Latency_2);
	if(*((u32*)(0x0800B000)) == 0xffffffff)
	{	
		FLASH_Unlock();
		FLASH_ErasePage(0x0800B000);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B000,0);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B004,0);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B008,0);
		FLASH_Lock();
	}
	if(!mag_zero_test_flag)//如果没有跳线连接，也就是不进入校准模式，就从flash读取旧的校准数据，继续运行
	{
		s32_x_mid = *((u32*)(0x0800B000));
		s32_y_mid = *((u32*)(0x0800B004));
		s32_z_mid = *((u32*)(0x0800B008));		
	}
	else //否则，进入校准模式，一直校准，直到拔掉跳线，就把校准值写入flash，并继续运行。
	{
		while( mag_zero_test_flag )
		{
			Mag_Read(Mag_Data);
			if(s32_z_max<Mag_Data[2]) 
				s32_z_max = Mag_Data[2];
			if(s32_z_min>Mag_Data[2]) 
				s32_z_min = Mag_Data[2];
			s32_z_mid = (s32_z_max+s32_z_min)/2;

			if(s32_x_max<Mag_Data[0]) 
				s32_x_max = Mag_Data[0];
			if(s32_x_min>Mag_Data[0]) 
				s32_x_min = Mag_Data[0];
			s32_x_mid = (s32_x_max+s32_x_min)/2;

			if(s32_y_max<Mag_Data[1]) 
				s32_y_max = Mag_Data[1];
			if(s32_y_min>Mag_Data[1]) 
				s32_y_min = Mag_Data[1];
			s32_y_mid = (s32_y_max+s32_y_min)/2;



			mag_zero_test_flag = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);	
		}
		FLASH_Unlock();
		FLASH_ErasePage(0x0800B000);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B000,s32_x_mid);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B004,s32_y_mid);
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x0800B008,s32_z_mid);
		FLASH_Lock();
	}
}	
