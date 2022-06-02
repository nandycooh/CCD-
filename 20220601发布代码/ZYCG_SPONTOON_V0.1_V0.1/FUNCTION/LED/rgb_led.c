#include "rgb_led.h"

/************************************************************  	
�������ƣ�	Led_Init
�������ܣ�	RGB ��ʼ��
��ڲ�����	void
���ز�����	��
˵����	    ��
***********************************************************/ 
void Led_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN | LED_BLUE_GPIO_PIN;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
}

void Led_ShutDown(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN | LED_BLUE_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ��GPIO��Ӧ�ٶ�    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //����Ϊ��� 
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);//��ʼ�� 
	
	BLUE_LED  = 1;
	GREEN_LED = 1;
	RED_LED   = 1;
}

/******************************************************************************************************  	
�������ƣ�	led_ctrl
�������ܣ�	RGB ��ɫ����
��ڲ�����	colour��
            			RED     1
            			GREEN   2
            			BLUE    3
            			YELLOW  4
            			PURPLE  5
            			NAVY    6
            			WHITE   7
            state��
            			LED_OFF   1
            			LED_ON    0
���ز�����	��
˵����	    ��
*****************************************************************************************************/
void led_ctrl(u8 colour, bool state)
{
	GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN | LED_BLUE_GPIO_PIN);
	switch(colour){
		case RED:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN);
			}
			else {
				GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN);
			}
			break;
		case GREEN:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_GREEN_GPIO_PIN);
			}
			else{
				GPIO_SetBits(LED_GPIO_PORT, LED_GREEN_GPIO_PIN);
			}
			break;
		case BLUE:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
			}
			else {
				GPIO_SetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
			}
			break;
		case YELLOW:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN);
			}
			else{
				GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN);
			}
			break;
		case PURPLE:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_RED_GPIO_PIN);
			}
			else{
				GPIO_SetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_RED_GPIO_PIN);
			}
			break;
		case NAVY:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_GREEN_GPIO_PIN);
			}
			else{
				GPIO_SetBits(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_GREEN_GPIO_PIN);
			}
			break;
		case WHITE:
			if(state){
				GPIO_ResetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN | LED_BLUE_GPIO_PIN);
			}
			else{
				GPIO_SetBits(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN | LED_BLUE_GPIO_PIN);
			}
			break;
		default:
			break;
	}
}

void led_green_turn(void)
{
	led_ctrl(GREEN, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_GREEN_GPIO_PIN));
}

void led_red_turn(void)
{
	led_ctrl(RED, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_RED_GPIO_PIN));
}

void led_blue_turn(void)
{
	led_ctrl(BLUE, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_BLUE_GPIO_PIN));
}

void led_white_turn(void)
{
	led_ctrl(WHITE, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN));
}

void led_yellow_turn(void)
{
	led_ctrl(YELLOW, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_RED_GPIO_PIN | LED_GREEN_GPIO_PIN));
}

void led_purple_turn(void)
{
	led_ctrl(PURPLE, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_RED_GPIO_PIN));
}

void led_navy_turn(void)
{
	led_ctrl(NAVY, (bool)GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_BLUE_GPIO_PIN | LED_GREEN_GPIO_PIN));
}

/************************************************************  	
�������ƣ�	led_turn_state
�������ܣ�	RGB ��˸״̬
��ڲ�����	colour��ָ����ɫ
���ز�����	��
˵����	    ��
***********************************************************/ 
void led_turn_state(u8 colour)
{
	switch(colour)
	{
		case RED	:	led_red_turn();break;
		case GREEN:	led_green_turn();break;
		case BLUE	:	led_blue_turn();break;
		case WHITE:	led_white_turn();break;
		case YELLOW:	led_yellow_turn();break;
		case PURPLE:	led_purple_turn();break;
		case NAVY:	led_navy_turn();break;
		default:
			break;
	}
}
/************************************************************  	
�������ƣ�	led_xxxx_state
�������ܣ�	ָ��RGB ������
��ڲ�����	status��״̬
���ز�����	��
˵����	    ��
***********************************************************/ 
void led_green_state(bool status)
{
	if(status == LED_OFF)
		led_ctrl(GREEN, (bool)LED_OFF);
	else
		led_ctrl(GREEN, (bool)LED_ON);
}

void led_red_state(bool status)
{
	if(status == LED_OFF)
		led_ctrl(RED, (bool)LED_OFF);
	else
		led_ctrl(RED, (bool)LED_ON);
}

void led_blue_state(bool status)
{
	if(status == LED_OFF)
		led_ctrl(BLUE, (bool)LED_OFF);
	else
		led_ctrl(BLUE, (bool)LED_ON);
}

void led_white_state(bool status)
{
	if(status == LED_OFF)
		led_ctrl(WHITE, (bool)LED_OFF);
	else
		led_ctrl(WHITE, (bool)LED_ON);
}

void led_yellow_state(bool status)
{
	if(status == LED_OFF)
		led_ctrl(YELLOW, (bool)LED_OFF);
	else
		led_ctrl(YELLOW, (bool)LED_ON);
}

/************************************************************  	
�������ƣ�	Led_Management
�������ܣ�	ϵͳ�ƹ�ָʾ����
��ڲ�����	��
���ز�����	��
˵����	    ��
***********************************************************/ 
void Led_Management(void)
{

}

LED_DRIVER_APP_API Led_Driver_App_Port = 
{
	.Driver_LedInit = Led_Init,
	.Driver_LedShutDown = Led_ShutDown,
	//LED��˸״̬
	.App_LedTurnState=led_turn_state,
	//LED����״̬
	.App_LedWhiteState=led_white_state,
	.App_LedBlueState=led_blue_state,
	.App_LedGreenState=led_green_state,
	.App_LedredState=led_red_state,
	//�ƹ����
	.App_LedManagement=Led_Management,
};
