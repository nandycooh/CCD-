#include "sleep.h"

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
	//SystemInit();
	delay_init();
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
//	LED_OFF;
	//��ʱ������Ҫ��ͣ
//	TIME_ControlStruct.stop(1);
//	TIME_ControlStruct.stop(2);
	/*α������˯��ģʽ*/
	//4438������ػ�ģʽ
	
//	RF_433_470.ShutDown();
	ADC_Cmd(ADC1, DISABLE);

	Sys_Enter_Stop();
	/*����*/
	__disable_irq();
	NVIC_SystemReset();
}

void ALL_GPIO_AN(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|
																GPIO_Pin_1|
																GPIO_Pin_2|
																GPIO_Pin_3|
																GPIO_Pin_4|
																GPIO_Pin_5|
																GPIO_Pin_6|
																GPIO_Pin_7|
//																GPIO_Pin_8|
																GPIO_Pin_9|
																GPIO_Pin_10|
																GPIO_Pin_11|
																GPIO_Pin_12|
																GPIO_Pin_13|
																GPIO_Pin_14|
																GPIO_Pin_15;
	//ѡ��GPIO��Ӧ�ٶ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
	//����Ϊ���           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	//��ʼ��    			
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|
																GPIO_Pin_1|
																GPIO_Pin_2|
//																GPIO_Pin_3|
																GPIO_Pin_4|
																GPIO_Pin_5|
																GPIO_Pin_6|
																GPIO_Pin_7|
																GPIO_Pin_8|
																GPIO_Pin_9|
																GPIO_Pin_10|
																GPIO_Pin_11;
//																GPIO_Pin_12|
//																GPIO_Pin_13|
//																GPIO_Pin_14|
//																GPIO_Pin_15;
	//ѡ��GPIO��Ӧ�ٶ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
	//����Ϊ���           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	//��ʼ��    			 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void Un_Used_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

static void Restore_Default_Settings(void)
{
	ALL_GPIO_AN();
	TIM_DeInit(TIM4);
	TIM_DeInit(TIM2);
	ADC_DeInit(ADC1);
	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);
}

static void Enter_Stop(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	SysTick->CTRL = 0x00;//�رն�ʱ��
	SysTick->VAL = 0x00;//���val,��ն�ʱ��;
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);//�ص�����������ǽ���STOP
	SystemInit();   //��ʼ��ϵͳʱ��
}

static  void Enter_LowPowerStop(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	SysTick->CTRL = 0x00;//�رն�ʱ��
	SysTick->VAL = 0x00;//���val,��ն�ʱ��;
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);//�ص�����������ǽ���STOP
	SystemInit();   //��ʼ��ϵͳʱ��
}

static void Enter_Standby(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  		  
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
    //ʹ���ⲿ�жϷ�ʽ
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ
}

void EXTI0_IRQHandler(void)
{    
	EXTI_ClearITPendingBit(EXTI_Line0);    
} 

static void Restart(void)
{
	__disable_irq();
	NVIC_SystemReset();
}
/**
  * @brief Ӳ��Sleepʵ���ҳ�ʼ��
  * @param
  * @retval: None
*/
HalAdapter_Management_OperatingMode_t halAdapter_OptMode = 
{
.isInit = true,
.HAL_EnterStop = Enter_Stop,
.HAL_EnterLowPowerStop = Enter_LowPowerStop,
.HAL_Restart = Restart,
.HAL_EnterStandby = Enter_Standby,
.HAL_RestoreDefaultSettings = Restore_Default_Settings,
};

static HalAdapter_Management_OperatingMode_t* HAL_adapterInit_OptMode(void)
{
	if(halAdapter_OptMode.isInit == false){
		//ʵ����
		//ALL_GPIO_AN();
		halAdapter_OptMode.isInit = true;
	}
	return &halAdapter_OptMode;
}
/**
  * @brief ��ȡӲ��Sleepʵ��
  * @param
  * @retval: None
*/
HalAdapter_Management_OperatingMode_t* GetHalAdapter_OptMode(void)
{
	return HAL_adapterInit_OptMode();
}
