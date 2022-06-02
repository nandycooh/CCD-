#include "../function/time/hal_timer.h"
#include "../function/led/rgb_led.h"
#include "../function/power/powerManage.h"
#include "../function/recoil/recoil.h"

/************************************************************  
FileName: hal_timer.c   
Author: hf             
Date: 2021.03.23
Description: tim��ʼ�����ײ�����        
Version: V 1.0  
Other:
***********************************************************/ 




u16 Tim4_Tim_Flg=0;//�������
u32 sleep_count=0;//���߼���

/**************************************************************   	
�������ƣ�	TIM2_Int_Init
�������ܣ�	tim2��ʼ��
��ڲ�����	arr���Զ���װֵ��
            psc��ʱ��Ԥ��Ƶ��
���ز�����	��
˵����	    ��
**************************************************************/ 
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
}

/**************************************************************   	
�������ƣ�	TIM4_Int_Init
�������ܣ�	tim4��ʼ��
��ڲ�����	arr���Զ���װֵ��
            psc��ʱ��Ԥ��Ƶ��
���ز�����	��
˵����	    ��
**************************************************************/ 
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
}

/**************************************************************   	
�������ƣ�	timer_start
�������ܣ�	��ʱ������
��ڲ�����	timerEx_id��ָ����ʱ�� �����趨2��
            timer_period����ʱ��ʱ�� ��λms
���ز�����	��
˵����	    ��
**************************************************************/ 
void timer_start(u8 timerEx_id,uint32_t timer_period)
{
	if(timerEx_id==1)
	{
		//���㣺72M/7200=10k--->10k=1s--->10=1ms�Դ����� ע�⣺arrΪu16��MAX65535
		TIM2_Int_Init(timer_period*10-1,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms
	}
	else if(timerEx_id==2)
	{
		TIM4_Int_Init((timer_period*10)-1,7199);
	}
}


/**************************************************************   	
�������ƣ�	timer_stop
�������ܣ�	timer��ͣ
��ڲ�����	timerEx_id��ָ����ͣ�Ķ�ʱ�� �����趨2��
���ز�����	��
˵����	    ��
**************************************************************/ 
void timer_stop(u8 timerEx_id)
{
	if(timerEx_id==1)
		TIM_Cmd(TIM2, DISABLE);
	if(timerEx_id==2)
		TIM_Cmd(TIM4, DISABLE);
	if(timerEx_id==3)
		TIM_Cmd(TIM3, DISABLE);
}
extern void RF_SI4438_Ack_Cmd_Data(void);
//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־
		Tim4_Tim_Flg++;
		sleep_count++;
	}
}

u8 recoil_abnormal;
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		recoil_abnormal++;
	}
}

/*�ⲿ����api��������*/
TIME_ControlDef TIME_ControlStruct = 
{
	.start = timer_start,
	.stop  = timer_stop,
};







