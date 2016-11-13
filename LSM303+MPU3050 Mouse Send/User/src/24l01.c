#include "24l01.h"
//#include "lcd.h"
//#include "delay.h"
//#include "usart.h"
//Mini STM32开发板
//NRF24L01 驱动函数 
	 

extern u8 TX_ADDRESS[TX_ADR_WIDTH]; //发送地址
extern u8 RX_ADDRESS[RX_ADR_WIDTH]; //接收地址

//SPI初始化
void SPI1_Init(void)
{	 
	//GPIO的配置结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//SPI的配置结构体
	SPI_InitTypeDef  SPI_InitStructure;
	//PORTA口和SPI1口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);	   
	//SPI1口初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//对应神舟Ⅰ号的PA5--SCK、PA6--MISO、PA7--MOSI
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;		//全双工模式
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;													//SPI主机
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;											//8bit数据格式
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;														//空闲状态时，SCK=0
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;													//数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;															//启用软件从设备管理
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8; 	  //8分频，spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
																																			//原来设置的256分频速度太慢，接不到数据
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;										//先发送MSB
	SPI_InitStructure.SPI_CRCPolynomial=7;															//CRC校验
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);	
}   
							    
//初始化24L01的IO口
void NRF24L01_Init(void)
{
//	u8 i,buf[5];
	//GPIO的配置结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能GPIOA和GPIOB的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	//PA3连接24L01的CE作为输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;//对应神舟Ⅰ号的PA3--CE,PA4-NSS
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB4输出，对应24L01的CSN
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//对应神舟Ⅰ号的PB4--CSN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI1_Init();    //初始化SPI1
	NRF24L01_it_irq();//NRF24L01中断配置
	
	CE_0; 	//chip enable
	CSN_1;	//spi disable
}
//24L01的中断配置
void NRF24L01_it_irq(void)
{
	//GPIO的配置结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	//外部中断配置结构体
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//GPIOA_2输入，连接24L01的IRQ中断引脚,并将PA2配置成外中断2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//对应神舟Ⅰ号的PA2--IRQ
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//GPIO_SetBits(GPIOA,GPIO_Pin_2);//初始状态置IRQ为高
	
	#ifdef VECT_TAB_FLASH
		/*将向量表起始地址设置为0x20000000*/
		NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	#else			/*VECT_TAB_FLASH*/
		/*将向量表起始地址设置为0x08000000*/
		NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif
	
	/*配置用1位表示优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/*允许EXTI2中断*/
	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//外部中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//上升下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	CSN_0;                 //使能SPI传输
  	status =SPI1_ReadWriteByte(reg);//发送寄存器号 
  	SPI1_ReadWriteByte(value);      //写入寄存器的值
  	CSN_1;                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	  u8 reg_val;	    
 	  CSN_0;          //使能SPI传输		
  	SPI1_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI1_ReadWriteByte(0xFF);//读取寄存器内容
  	CSN_1;          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}	
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  (等待接收缓冲区为空)
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据				
}
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	CSN_0;           //使能SPI传输
  	status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
		for(u8_ctr=0;u8_ctr<len;u8_ctr++)
				pBuf[u8_ctr]=SPI1_ReadWriteByte(0xFF);//读出数据
  	CSN_1;       //关闭SPI传输
  	return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
		u8 status,u8_ctr;	    
		CSN_0;          //使能SPI传输
  	status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //写入数据	 
  	CSN_1;       //关闭SPI传输
  	return status;          //返回读到的状态值
}				   
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;   
	CE_0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
  CE_1;//启动发送	   
	while(NRF24L01_IRQ!=0) ;//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
//启动NRF24L01接收一次数据
//rxbuf:待接收数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							    
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		CE_0;
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		CE_1;
		return 0; 
	}	   
	return 1;//没收到任何数据
}					    
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(void)
{	
	  CE_0;
		NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x70);				//clear RX_OK or TX_OK or MAX_TX interrupt flag
		NRF24L01_Write_Reg(FLUSH_TX,0);		//清空TX_FIFO
		NRF24L01_Write_Reg(FLUSH_RX,0);		//清空RX_FIFO
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
		NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答 
		NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     //设置RF通信频率		  
		NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 		
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);//PLL_LOCK允许,数据传输率1Mbps,发射功率0dBm,低噪声放大器增益开启   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	CE_1; //CE为高,进入接收模式 
}						 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void TX_Mode(void)
{													 
	  CE_0;	    
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x32);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);       //设置RF通道为40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发送模式,开启所有中断
	  CE_1;//CE为高,10us后启动发送
}		  

//检测24L01是否存在
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i; 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //读出写入的地址  	
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0XA5)break;
		//USART_SendData(USART1,buf1[i]);
	}
	if(i!=5)return 1;//检测24L01错误
	return 0;		 //检测到24L01
}	





