#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "func_config.h"

/*********************���IICʹ�õĺ�****************************/

//
#define Soft_I2C_SDA 		GPIO_Pin_7
#define Soft_I2C_SCL 		GPIO_Pin_6
#define Soft_I2C_PORT   GPIOB
//
#define Soft_I2C_SCL_0 		GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SCL)
#define Soft_I2C_SCL_1 		GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SCL)
#define Soft_I2C_SDA_0 		GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SDA)
#define Soft_I2C_SDA_1   	GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SDA)

//
#define Soft_I2C_SDA_STATE   	GPIO_ReadInputDataBit(Soft_I2C_PORT, Soft_I2C_SDA)
#define Soft_I2C_DELAY 				Soft_I2C_Delay(100000)
#define Soft_I2C_NOP					Soft_I2C_Delay(10) 
//
#define Soft_I2C_READY		0x00
#define Soft_I2C_BUS_BUSY	0x01	
#define Soft_I2C_BUS_ERROR	0x02
//
#define Soft_I2C_NACK	  0x00 
#define Soft_I2C_ACK		0x01

void Soft_I2C_Configuration(void);

int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, 
                                          unsigned short RegisterLen, unsigned char *RegisterValue);
int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, 
                                           unsigned short RegisterLen, const unsigned char *RegisterValue);

void MPU_Delay_Ms(u16 ms);
void MPU_Delay_Us(u32 us);
void MPU_Get_Ms(unsigned long *time);
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�
 
#endif
















