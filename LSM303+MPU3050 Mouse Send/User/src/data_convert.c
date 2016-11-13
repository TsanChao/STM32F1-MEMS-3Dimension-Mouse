/***********************************************************************************************************************************
**文件名.convert.c
**功能：四元数姿态数据到鼠标需要的坐标偏移量的转换以及数据格式的封装
***********************************************************************************************************************************/
#include "data_convert.h"
#include "math.h"
#include "ahrs_states.h"
#include "24l01.h"
#include "usart.h"
#include "key.h"
 
//将四元数转换为欧拉角，以角度偏移值代替坐标偏移值
//适当将角度放大，映射到整个屏幕
//此算法参见《捷联惯性导航技术》(David H.Titterton John L.Weston)欧拉角、四元数、方向余弦的相互转化部分
//算法是以飞机的姿态坐标系展开，发送数据时对应到鼠标的坐标系上
//飞机的姿态坐标系参见AN3192
int quat2Eular(float *quat,float *euler)
{
	u8 i;
	u8 flag = 1;	//处于框架自锁状态时，单独更新Phi或Psi的标志位
	static float PI = 3.141593;	
	float a,b,c,d;	//a,b,c,d对应四元数的四个值
	float Phi,			//大写的Phi即φ，飞机绕x轴旋转表示横滚角
				Theta,		//大写的Theta即θ，飞机绕y轴旋转表示俯仰角
				Psi;			//大写的Psi即ψ，飞机绕z轴旋转表示偏航角
	float phi,			//小写的phi表示偏移量
				theta,
				psi;
	float avg;
	avg=sqrt(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
	for(i=0;i<4;i++)
	{
		quat[i]=quat[i]/avg;
	}
	
	//将quat[4]分别赋值给a,b,c,d
	a=quat[0];
	b=quat[1];
	c=quat[2];
	d=quat[3];
	//计算欧拉角，同时判断是否达到框架自锁
	//飞机绕y轴转，对应Theta
	Theta=asin(2*a*c-2*b*d);  
	//下述判断为了使Theta在+-90度附近不产生突跳
	if(fabs(PI/2 - Theta) < 0.1 || fabs(-PI/2 - Theta) < 0.1)
	{
		//单独更新ψ和φ,Phi和Psi
		if(flag)
		{
			Phi = Phi;
			Psi = atan((2*c*d-2*a*b+2*b*c-2*a*d)/(2*(b*d+a*c)-(a*a-b*b+c*c-d*d))) - Phi;	//ψ=arctan[(c23+c12)/(c13-c22)]-φ
		}
		else 
		{
			Psi = Psi ;
			Phi = atan((2*c*d-2*a*b+2*b*c-2*a*d)/(2*(b*d+a*c)-(a*a-b*b+c*c-d*d))) - Psi;	//φ=arctan[(c23+c12)/(c13-c22)]-ψ
		}
		flag = !flag ; //flag取反
	}
	else 	//(fabs(PI/2 - Theta) >= 0.1 && fabs(-PI/2 - Theta) >= 0.1)	θ不接近+-90度，亦即θ不处于框架自锁区域
	{
		//飞机左右旋转时对应Psi
		Psi=atan(2*(b*c+a*d)/(a*a+b*b-c*c-d*d));
		//飞机横向滚动时对应Phi
		Phi=atan(2*(c*d+a*b)/(a*a-b*b-c*c+d*d));
	}

	//计算角度相对上次的偏移量
	theta = Theta - gStateData.theta;
	psi   = Psi   - gStateData.psi;
	phi   = Phi   - gStateData.phi;
	
	//将角度放大，映射到屏幕上的移动
	//此处的数据要对应到鼠标的坐标系上，此处不要与飞机的坐标系一一对应
	euler[0] = psi   * 1000;				//鼠标左右旋转对应飞机的Psi
	euler[1] = -phi  * 1000;				//鼠标上下移动对应飞机的Phi   
	euler[2] = theta * 1000;				//功能暂未定义
	
	//此处处理将小波动忽略，以免鼠标不动时指针抖动
	for(i=0;i<2;i++)
	{
		if(fabs(euler[i]) < 3)
			euler[i] = 0;
	}
	
	
	//对结构体中欧拉角赋值，进行下一次计算
	gStateData.theta = Theta;
	gStateData.psi   = Psi;
	gStateData.phi   = Phi;
	
	return 0;
}


void Send_AHRS_Data(void)
{
	int i;
	s8 data[6];
	float quad[4];
 	float move[3];//float坐标偏移
	
	quad[0] = gStateData.qib.a;  
	quad[1] = -gStateData.qib.b; 
	quad[2] = -gStateData.qib.c; 
	quad[3] = gStateData.qib.d;
	
	//quaternion_to_coord(coord1,quad,coord2);
  quat2Eular(quad,move);
	//data[]是封装后的数据包	
	data[0] = 0x80;
	data[1] = 0x0A;
	data[2] = key_scan();
	data[3] = (s8)move[0];
	data[4] = (s8)move[1];
	data[5] = 0x00;
	
 	NRF24L01_TxPacket((u8*)data);
	for(i=0;i<6;i++)
	{
		printf("%c",data[i]);
	}	
}

