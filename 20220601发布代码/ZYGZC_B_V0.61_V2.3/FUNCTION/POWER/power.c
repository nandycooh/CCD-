#include "power.h"

/*********************************Ӳ��������****************************/
extern HalAdapter_Management_Led_t halAdapter_Led;//API�ӿڹ��ⲿʹ��

static u8 shutdown_flg=0;

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SI4438_POWER_GPIO_PIN|RS485_POWER_GPIO_PIN | POWER_STATU_GPIO_PIN; //485��12V��Դ��ʼ��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = POWER_ON_OFF_GPIO_PIN; //485��12V��Դ��ʼ��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Power_OUT_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = POWER_ON_OFF_GPIO_PIN; //485��12V��Դ��ʼ��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,  POWER_ON_OFF_GPIO_PIN);
}


u8 Power_Shut_Down(u8 shutime)
{
	if(!POWER_ONOFF_STATU_PIN_STATE)//��Դ���ذ���
	{
		shutdown_flg++;
		halAdapter_Led.HAL_LedBlueTurn();

		if(shutdown_flg>=shutime)
		{	
			halAdapter_Led.HAL_LedShutDown();
			POWER_PIN_STATE=0;
			while(1);
		}
		return 1;
	}
	else
		shutdown_flg=0;
}


