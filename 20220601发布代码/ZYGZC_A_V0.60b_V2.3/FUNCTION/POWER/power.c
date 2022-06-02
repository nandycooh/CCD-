#include "power.h"

/*********************************Ӳ��������****************************/

/************************************************************  	
�������ƣ�	Power_Init
�������ܣ�	��Դ��س�ʼ��
��ڲ�����	void
���ز�����	��
˵����	    ��
***********************************************************/ 
void Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SI4438_POWER_GPIO_PIN;   //4438��Դ��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//232��Դ�͸�λ���Ƴ�ʼ��
	GPIO_InitStructure.GPIO_Pin = RS232_POWER_STATU_GPIO_PIN |	RS232_RESET_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

