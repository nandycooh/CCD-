#ifndef __HAL_IIC_H
#define __HAL_IIC_H

#include "sys.h"
#include "stdio.h"
#include "stdbool.h"

/***************************IIC********************************/
//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) 		//SCL
#define IIC_SDA    PBout(7) 		//SDA	 
#define READ_SDA   PBin(7) 		//����SDA 

//IIC������
typedef struct
{
	bool isInit;
	/**
		* @brief  IIC��ʼ��
		* @param  None
		* @retval : None
	**/
	void (*HAL_IIC_Init)(void);
	/**
		* @brief  IIC��ʼ�ź�
		* @param  None
		* @retval : None
	**/
	void (*HAL_IIC_Start)(void);
	/**
		* @brief  IICֹͣ�ź�
		* @param  None
		* @retval : None
	**/
	void (*HAL_IIC_Stop)(void);
	/**
		* @brief  IIC�ȴ�Ӧ��
		* @param  None
		* @retval : None
	**/
	u8 (*HAL_IIC_Wait_Ack)(void);
	/**
		* @brief  IICӦ��
		* @param  None
		* @retval : None
	**/
	void (*HAL_IIC_Ack)(void);
	/**
		* @brief  IIC��Ӧ��
		* @param  None
		* @retval : None
	**/
	void (*HAL_IIC_NAck)(void);
	/**
		* @brief  IICд����
		* @param  �����ֽ�
		* @retval : None
	**/
	void (*HAL_IIC_Write_Byte)(u8);
	
	/**
		* @brief 	IIC������
		* @param  �Ƿ�Ӧ��
		* @retval : �����ֽ�
	**/
	u8 (*HAL_IIC_Read_Byte)(unsigned char);

	
}HalAdapter_Management_IIC_t;

HalAdapter_Management_IIC_t* GetHalAdapter_IIC(void);
	  
#endif
















