#include "hal_iic.h"
#include "delay.h"
 
  //MPU IIC ��ʱ����
static void IIC_Delay(void)
{
	halAdapter_Clock.HAL_DelayUS(2);
}

//��ʼ��IIC
static void IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��ʹ������IO PORTCʱ�� 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
  GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 //PB10,PB11 �����	
 
}
//����IIC��ʼ�ź�
static void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	IIC_Delay();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
static void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	IIC_SCL=1;  
	IIC_SDA=1;//����I2C���߽����ź�
	IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;
	IIC_Delay();	   
	IIC_SCL=1;
	IIC_Delay();	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
static void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}
//������ACKӦ��		    
static void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	IIC_Delay();
	IIC_SCL=1;
	IIC_Delay();
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
static void IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	SDA_OUT(); 	    
  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
    IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	  
		IIC_SCL=1;
		IIC_Delay(); 
		IIC_SCL=0;	
		IIC_Delay();
  }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
static u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
    IIC_SCL=0; 
    IIC_Delay();
		IIC_SCL=1;
    receive<<=1;
    if(READ_SDA)receive++;   
		IIC_Delay(); 
  }					 
  if (!ack)
    IIC_NAck();//����nACK
  else
    IIC_Ack(); //����ACK   
  return receive;
}



HalAdapter_Management_IIC_t halAdapter_IIC = 
{
.isInit = false,
.HAL_IIC_Init = IIC_Init,
.HAL_IIC_Start = IIC_Start,
.HAL_IIC_Stop = IIC_Stop,
.HAL_IIC_Wait_Ack = IIC_Wait_Ack,
.HAL_IIC_Ack = IIC_Ack,
.HAL_IIC_NAck = IIC_NAck,
.HAL_IIC_Write_Byte = IIC_Send_Byte,
.HAL_IIC_Read_Byte = IIC_Read_Byte,
};
/**
  * @brief Ӳ��IICʵ���ҳ�ʼ��
  * @param
  * @retval: None
*/
static HalAdapter_Management_IIC_t* HAL_adapterInit_IIC(void)
{
	if(halAdapter_IIC.isInit == false){
		//ʵ����
		halAdapter_IIC.HAL_IIC_Init();
		halAdapter_IIC.isInit = true;
	}
	return &halAdapter_IIC;
}
/**
  * @brief ��ȡӲ��ADCʵ��
  * @param
  * @retval: None
*/
HalAdapter_Management_IIC_t* GetHalAdapter_IIC(void)
{
	return HAL_adapterInit_IIC();
}














