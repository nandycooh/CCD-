#include "../FUNCTION/PERIPHERAL/peripheral.h"
#include "../FUNCTION/LED/rgb_led.h"
#include "../function/protocol/protocol.h"
#include "../function/time/hal_timer.h"

/************************************************************  
FileName: peripheral.c   
Author: hf             
Date: 2021.03.23
Description: ��ǹ���������ʼ��������        
Version: V 1.0  
Other:
***********************************************************/ 

uint8_t gun_statusflg[2]={0};//��ǹ���ݼ��ϴ�״̬
u8 bullet_count=0;//�������
u8 recoil_stop;//�������Ͳ��������λ 211111

extern u32 sleep_count;
extern u16 Tim4_Tim_Flg;

/**************************************************************   	
�������ƣ�	Ziyuan_GunState_Init
�������ܣ�	�����ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Ziyuan_GunState_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	RCC_APB2PeriphClockCmd(TRIGGER_GPIO_CLK | GUN_REST_GPIO_CLK | CHAMBERING_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= TRIGGER_GPIO_PIN;//�����ʼ��
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(TRIGGER_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= INSURANCE_GPIO_PIN;//���ճ�ʼ��
	GPIO_Init(INSURANCE_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GUN_REST_GPIO_PIN;//��λ��ʼ��
	GPIO_Init(GUN_REST_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= CHAMBERING_GPIO_PIN;//���ų�ʼ��
	GPIO_Init(CHAMBERING_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= UP_CLIP_GPIO_PIN;//�ϵ��г�ʼ��
	GPIO_Init(UP_CLIP_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= DOWN_CLIP_GPIO_PIN;//�˵��г�ʼ��
	GPIO_Init(DOWN_CLIP_GPIO_PORT, &GPIO_InitStructure);
}

/**************************************************************   	
�������ƣ�	Ziyuan_Touch_Init
�������ܣ�	���������Դ���Ƴ�ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/
void Ziyuan_Touch_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(TOUCH_GPIO_CLK, ENABLE);
	//GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
	
	GPIO_InitStructure.GPIO_Pin 	= TOUCH_POWER_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(TOUCH_POWER_GPIO_PORT, &GPIO_InitStructure);
	
	//��Դ��ʼ��
	GPIO_InitStructure.GPIO_Pin 	= TOUCH_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 
		
	TOUCH_POWER=1;//Touch����
}

//�ⲿ�ж�8������� 
void EXTI9_5_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line8); //���LINE8�ϵ��жϱ�־λ   
}

/**************************************************************   	
�������ƣ�	Gun_All_Status
�������ܣ�	��ǹ״̬����
��ڲ�����	��
���ز�����	��
˵����	    0��δ���»�δ���� 1�����»򴥷�
**************************************************************/

void Gun_All_Status(void)
{
	/*********�˵�************/
	if(DOWN_CLIP_BUTTON)								//�˵�δ����
		gun_statusflg[0] &= (~0x40);
	else
	{
		gun_statusflg[0] |= 0x40;						//�˵�����
		gun_statusflg[1]=0x01;
	}
		
	/*********����************/
	if(!TOUCH_BUTTON)										//�޴���
	{
		gun_statusflg[0]&=(~0x80);
	}
	else
	{
		gun_statusflg[0]|=0x80;							//����
		sleep_count=0;										//���˯�߼���
	}
	
	/*********����************/	
	if(INSURANCE_BUTTON)								//����δ���� ��1
	{
		gun_statusflg[0]|=0x20;						
	}		
	else//��������
	{	
		gun_statusflg[0]&=(~0x20);						//���հ��� ��0
	}
	
	/*********���************/
	if(!TRIGGER_BUTTON)//����ɿ�
	{	
		gun_statusflg[0] &= (~0x10);
		
		if(Tim4_Tim_Flg>500)
		{
			if(!CHAMBERING_BUTTON)					//δ���� 
				gun_statusflg[0] &= (~0x02);
			else
			{
				gun_statusflg[0] |= 0x02;				//����
			}
		}
	}			
	else
	{
			Tim4_Tim_Flg=0;//�������� ����Ͳ��λ
			
			recoil_stop=1;
			gun_statusflg[0] |= 0x10;						//�������
			
			gun_statusflg[1]=0x01;
	}
	
	/*********�ϵ�************/
	if(UP_CLIP_BUTTON)									//�ϵ�δ����
		gun_statusflg[0]&=(~0x08);
	else
	{
		gun_statusflg[0]|=0x08;							//�ϵ�����
		gun_statusflg[1]=0x01;
	}
	
	/*********��λ************/
	if(REST_BUTTON)											//δ��λ
		gun_statusflg[0]&=(~0x04);
	else
	{
		gun_statusflg[0]|=0x04;
		gun_statusflg[1]=0x01;
	}
	
	if(!DOWN_CLIP_BUTTON || TOUCH_BUTTON || !INSURANCE_BUTTON || TRIGGER_BUTTON || CHAMBERING_BUTTON || !UP_CLIP_BUTTON || !REST_BUTTON)
		LED_ON;
	else
		LED_OFF;
}
