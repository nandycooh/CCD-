#include "../function/recoil/recoil.h"
#include "../FUNCTION/TIME/hal_timer.h"
#include "../function/peripheral/peripheral.h"

/************************************************************  
FileName: recoil.c   
Author: hf             
Date: 2021.03.23
Description: ��ǹ���ų�ʼ��������        
Version: V 1.0  
Other:
***********************************************************/ 

u32 time_count = 0;//����ʱ�����
u8 banji_flag = 0;
static u8 flg=0;//��������ɿ��밴�±�־λ

extern u8 bullet_count;
extern u8 recoi_mode;

/**************************************************************   	
�������ƣ�	GPIO_Configuration
�������ܣ�	���������ֳ�ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void GPIO_Configuration(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA| \
		RCC_APB2Periph_GPIOC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//LIN1~LIN3
   GPIO_Init(GPIOB, &GPIO_InitStructure);
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//HIN1~HIN3
   GPIO_Init(GPIOA, &GPIO_InitStructure); 

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//HADD1/HADD2
   GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//HADD3
   GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**************************************************************   	
�������ƣ�	TIM3_Int_Init
�������ܣ�	ͨ�ö�ʱ��3�жϳ�ʼ��
��ڲ�����	arr���Զ���װֵ��
            psc��ʱ��Ԥ��Ƶ��
���ز�����	��
˵����	    ��
**************************************************************/ 
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���					 
}

/**************************************************************   	
�������ƣ�	TIME1_PWM_Configuration
�������ܣ�	��ʱ��1����ͨ����ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void TIME1_PWM_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;//����Ƚ�
  TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
 
  TIM_TimeBaseStructure.TIM_Period = 2000;//�Զ����¼���ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =2;//Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; ////����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned2; //����ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//ָ���ظ���������ֵ
  TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure); 
 
  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;//����Ƚ�״̬
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;//��������Ƚ�״̬
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//ָ���������
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//ָ�������������  ���ԡ�TIM1���͡�TIM8����Ч
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;//ָ������״̬�µ�TIM����Ƚ�����״̬�� ���ԡ�TIM1���͡�TIM8����Ч
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//ָ������״̬�µ�TIM����Ƚ�����״̬  ���ԡ�TIM1���͡�TIM8����Ч
 
  TIM_OCInitStructure.TIM_Pulse = 5;//ָ��Ҫ���ص�����ȽϼĴ���������ֵ 
  TIM_OC1Init(ADVANCE_TIM,&TIM_OCInitStructure);
  TIM_OC1PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);
 
  TIM_OCInitStructure.TIM_Pulse = 5; 
  TIM_OC2Init(ADVANCE_TIM,&TIM_OCInitStructure);
  TIM_OC2PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);
	
  TIM_OCInitStructure.TIM_Pulse = 5; 
  TIM_OC3Init(ADVANCE_TIM,&TIM_OCInitStructure);
  TIM_OC3PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);	
 
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//������ģʽ��ʹ�õ�off״̬ѡ��
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//����״̬��ʹ�õ�off״̬
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;//�����������
  TIM_BDTRInitStructure.TIM_DeadTime = 0x80;//���л����ر�֮����ӳ�ʱ��
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//�Ƿ�����TIM Break����
  //TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//TIM Break�������ż���
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Ƿ�����TIM�Զ��������
  TIM_BDTRConfig(ADVANCE_TIM,&TIM_BDTRInitStructure);
 
  TIM_Cmd(ADVANCE_TIM, ENABLE);  
  TIM_CtrlPWMOutputs(ADVANCE_TIM,ENABLE);
}

/**************************************************************   	
�������ƣ�	Commutation
�������ܣ�	����
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Commutation(void)
{
	 u8 h_io = 0;//�߲�����
   h_io = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);		
	 h_io = h_io + (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)<<1);//1<<1 �ȼ��� 1*2^1=2
   h_io = h_io + (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)<<2);//1<<2 �ȼ��� 1*2^2=4
	 
	switch(h_io)//�жϻ���λ��
	{
		case 0x02:   //CA
		{TIM1->CCER = 0x0108;};break;
		case 0x06:   //CB  
		{TIM1->CCER = 0x0180;};break;
		case 0x04:   //AB  
		{TIM1->CCER = 0x0081;};break;
		
		case 0x05:   //AC
		{TIM1->CCER = 0x0801;};break;
		case 0x01:   //BC    
		{TIM1->CCER = 0x0810;};break;
		case 0x03:   //BA 
		{TIM1->CCER = 0x0018;};break;
			default:break;
	}
	
//	switch(h_io)//ԭ����ǹ 210426
//	{
//		case 0x04:   //BA  1.4
//		{TIM1->CCER = 0x0018;};break;
//		case 0x06:   //BC  4.5
//		{TIM1->CCER = 0x0810;};break;
//		case 0x02:   //AC  2.5
//		{TIM1->CCER = 0x0801;};break;
//		case 0x03:   //AB  2.3
//		{TIM1->CCER = 0x0081;};break;
//		case 0x01:   //CB  3.6    
//		{TIM1->CCER = 0x0180;};break;
//		case 0x05:   //CA  6.1
//		{TIM1->CCER = 0x0108;};break;
//			default:break;
//	}
}

/**************************************************************   	
�������ƣ�	Recoil_Init
�������ܣ�	���������ֳ�ʼ����װ
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Recoil_Init(void)
{
	//���㣺72M/7200=10k--->10k=1s--->10=1ms�Դ����� ע�⣺arrΪu16��MAX65535
	//T=��TIM_Period+1��*��TIM_Prescaler+1��/TIMxCLK
	TIM3_Int_Init(1,35999);
	GPIO_Configuration(); //IO
  TIME1_PWM_Configuration(); //
}

/**************************************************************   	
�������ƣ�	TIM3_IRQHandler
�������ܣ�	��ʱ���жϣ��жϰ���Ļ������
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		
		if(!recoi_mode)//Ӧ��ģʽ
		{
			if(bullet_count == 1)//�ۼ�ʱ�� ������¾ͻ���
			{
				TIM_Cmd(TIM1,ENABLE);
			  time_count++;
				Commutation();//����
			}
			
			if(time_count >=120)//����70�λ���һ�� �൱�ڻ���ʱ��
			{
				TIM1->CCER = 0;//���
				bullet_count=0;
				TIM_Cmd(TIM1,DISABLE);
			}
		}
		else//���ؿ���ģʽ
		{
			if(TRIGGER_BUTTON && flg==0)//�������
			{
				TIM_Cmd(TIM1,ENABLE);
				banji_flag = 1;//������±�־
				time_count = 0;
				flg=1;
			}			
			
			if(banji_flag == 1)//�ۼ�ʱ�� ������¾ͻ���
			{
			  time_count++;
				Commutation();//����
			}
			
			if(time_count >=120)//����70�λ���һ�� �൱�ڻ���ʱ��
			{
				TIM1->CCER = 0;//���
				TIM_Cmd(TIM1,DISABLE);
				
				banji_flag=0;/*��ֹ�������һֱͨ�� �������ڰ���ɿ��ж��ڣ�������Ч*/
				
				if(!TRIGGER_BUTTON && flg==1)//����ɿ� ���δ���
				{
//					bullet_count++;//��������
//					
//					if(bullet_count>=100)
//					{
//						
//						bullet_count=0;
						flg =0;
						time_count = 0;//���¼�ʱ
//					}
				}
			}
		}
	}
}

