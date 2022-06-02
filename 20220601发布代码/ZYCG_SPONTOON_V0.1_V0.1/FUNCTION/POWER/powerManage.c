#include "powerManage.h"
#include "rgb_led.h"
#include "hal_adc.h"
#include "hal_timer.h"
#include "rgb_led.h"
/************************************************************  
FileName: powerManage.c   
Author: hf             
Date: 2021.03.23
Description: ��Դ���ֳ�ʼ��������        
Version: V 1.0  
Other:
***********************************************************/ 

u8 bat_Capacity;//����
static u8 shutdown_flg=0;

extern LED_DRIVER_APP_API Led_Driver_App_Port;//API�ӿڹ��ⲿʹ��

//��ʼ��
static void Power_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//�ͷ�JTAG����  ʹ�������PB3/4 ��PA15��Ϊ��ͨI/O JTAG-DP�����ã�SW-DP������
	
	//USB��Դ�ܽ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//���ػ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��Դ����IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	POWER_ON_OFF=0;
	//���IC�����ܽ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   //�ڲ���USB���жϣ�����У���   �������� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



u8 Power_Shut_Down(u8 shutime)
{
	if(!KEY_02)//��Դ���ذ���
	{
		shutdown_flg++;
		Led_Driver_App_Port.App_LedTurnState(BLUE);//red��˸ 1S/��

		if(shutdown_flg>=shutime)
		{	
			Led_Driver_App_Port.Driver_LedShutDown();
			POWER_ON_OFF=0;
		}
		return 1;
	}
	else
		shutdown_flg=0;
	
	return 0;
}

void Disable_Charge_Interrupt(void){
	EXTI->IMR &= ~(EXTI_Line11); 
}

void Enable_Charge_Interrupt(void){
	EXTI->IMR |= EXTI_Line11;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line11);    
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line12);    
	}
}

//�Ƿ��ڳ��
uint8_t Get_ChargePinStatus(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
}

//�����Ƿ����
uint8_t Get_FullBatteryPin_InCharge(void)
{
		return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
}



static float voltage = 0;
/**************************************************************   	
�������ƣ�	charge_led_dsp
�������ܣ�	��ع���
��ڲ�����	��
���ز�����	��
˵����	    �����RGB�� �� ����Դ���Ʋ��֣�
						�����----��ɫ����----�͵�ƽ
            ������---��ɫ����----�ߵ�ƽ
            δ���----�ߵ�ƽ
**************************************************************/ 
uint8_t Get_battery_power(void)
{
    
    //����ʵ�ʲ�����������4V��ADC�ӿڴ�2V����
    voltage = 2*calculate_bat();//������ȡ
//    printf("voltage = %f\r\n", voltage);
    if(voltage >= BAT_CAPACITY_LV1)  //100-80
    {
        bat_Capacity = 100;
    }
    else if((voltage < BAT_CAPACITY_LV1) && (voltage >= BAT_CAPACITY_LV2))  //80-60
    {
        bat_Capacity = 80;
    }
    else if((voltage < BAT_CAPACITY_LV2) && (voltage >= BAT_CAPACITY_LV3))  //60-40
    {
        bat_Capacity = 60;
    }
    else if((voltage < BAT_CAPACITY_LV3) && (voltage >= BAT_CAPACITY_LV4))  //40-20
    {
        bat_Capacity = 40;
    }
    else if((voltage < BAT_CAPACITY_LV4) && (voltage >= BAT_CAPACITY_LV5))  //20-0
    {
        bat_Capacity = 20;
    }
    else if(voltage < BAT_CAPACITY_LV5)//����
    {
        bat_Capacity = 1;        
    }
		
	return bat_Capacity;
}

/**
  * @brief Ӳ��Powerʵ���ҳ�ʼ��
  * @param
  * @retval: None
*/
HalAdapter_Management_Power_t halAdapter_Power = 
{
.isInit = false,
.HAL_Power_Init = Power_Init,
.HAL_PowerCharge_EnableInterrupt = Enable_Charge_Interrupt,
.HAL_PowerCharge_DisableInterrupt = Disable_Charge_Interrupt,
.HAL_Get_ChargePinStatus = Get_ChargePinStatus,
.HAL_Get_FullBatteryPin_InCharge = Get_FullBatteryPin_InCharge,
.HAL_Get_battery_power =Get_battery_power,
};
