#include "../function/led/rgb_led.h"
#include "../function/power/powerManage.h"

/************************************************************  
FileName: rgb_led.c   
Author: hf             
Date: 2021.03.23
Description: LED��ʼ�����ײ�����        
Version: V 1.0  
Other:
***********************************************************/ 

/*********************************Ӳ��������****************************/
/**************************************************************   	
�������ƣ�	Led_Init
�������ܣ�	LED��ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

/*********************************����ʵ�ֲ�****************************/
/**************************************************************   	
�������ƣ�	led_ctrl
�������ܣ�	LED��˸
��ڲ�����	times��1������˸����
���ز�����	��
˵����	    ��
**************************************************************/ 
void led_ctrl(u8 times)
{
	LED_PIN_PORT = !LED_PIN_PORT;
	delay_ms(1000/times);
}

LED_ControlDef LED_ControlStructf = 
{
	.Driver_LedInit = Led_Init,
};
