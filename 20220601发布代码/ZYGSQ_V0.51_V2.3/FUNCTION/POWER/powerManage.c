#include "../function/power/powerManage.h"
#include "../function/led/rgb_led.h"
#include "../function/time/hal_timer.h"
#include "../function/si4438/drv_SI4438.h"
#include "../function/recoil/recoil.h"

/************************************************************  
FileName: powerManage.c   
Author: hf             
Date: 2021.03.23
Description: ��Դ���ֳ�ʼ��������        
Version: V 1.0  
Other:
***********************************************************/ 

extern TIME_ControlDef TIME_ControlStruct;

/**************************************************************   	
�������ƣ�	Sys_Standby
�������ܣ�	����ģʽ��ֻ��ʱ�Ӻ�PA0����
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
static void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 		 
}

/**************************************************************   	
�������ƣ�	Sys_Enter_Standby
�������ܣ�	ϵͳ�������ģʽ
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Sys_Enter_Standby(void)
{	
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	Sys_Standby();
}

/**************************************************************   	
�������ƣ�	Sys_Enter_Stop
�������ܣ�	ֹͣģʽ������ͨ�������ⲿ�жϻ���
��ڲ�����	��
���ز�����	��
˵����	    ��Ϊʹ���˶�ʱ���жϻ������ȼ�һ��Ҫ��ߣ������޷�����
**************************************************************/ 
void Sys_Enter_Stop(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
	//���Ѻ�Ӵ˴���ʼ��������
	SystemInit();
	//�ָ��رյĹ���
	SI44xx_Power_Init();//�ȹ�����ܳ�ʼ��
	delay_ms(1000);//�ȴ�������
	
	SI446x_Init();
	Recoil_Init();
	TIME_ControlStruct.start(1,1000);								//��ʼ����ʱ��2 1S ˯�߼���
	TIME_ControlStruct.start(2,1);
}


/**************************************************************   	
�������ƣ�	Enter_Sleep
�������ܣ�	ϵͳ����͹���ģʽ
��ڲ�����	��
���ز�����	��
˵����	    ����ѡ�����ֹͣģʽ������ͨ�������ⲿ�жϻ���
**************************************************************/ 
void Enter_Sleep(void)
{
	//��ʱ������Ҫ��ͣ
	TIME_ControlStruct.stop(1);
	TIME_ControlStruct.stop(2);
	TIME_ControlStruct.stop(3);
	//433����
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//4438��Դ��
	//����
	LED_OFF;
	
	Sys_Enter_Stop();
	
//	/*����*/
//	__disable_irq();
//	NVIC_SystemReset();
}
