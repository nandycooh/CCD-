#ifndef __PERIPHERAL_H
#define __PERIPHERAL_H	

#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "sys.h"
#include "stm32f10x.h"

//����
#define INSURANCE_GPIO_PORT			GPIOA								
#define INSURANCE_GPIO_CLK			RCC_APB2Periph_GPIOA
#define INSURANCE_GPIO_PIN			GPIO_Pin_11
//���
#define TRIGGER_GPIO_PORT				GPIOA								
#define TRIGGER_GPIO_CLK				RCC_APB2Periph_GPIOA
#define TRIGGER_GPIO_PIN				GPIO_Pin_4
//��λ
#define GUN_REST_GPIO_PORT			GPIOB								
#define GUN_REST_GPIO_CLK				RCC_APB2Periph_GPIOB
#define GUN_REST_GPIO_PIN				GPIO_Pin_11
//����
#define CHAMBERING_GPIO_PORT		GPIOC								
#define CHAMBERING_GPIO_CLK			RCC_APB2Periph_GPIOC
#define CHAMBERING_GPIO_PIN			GPIO_Pin_13
//�ϵ���
#define UP_CLIP_GPIO_PORT				GPIOC								
#define UP_CLIP_GPIO_CLK				RCC_APB2Periph_GPIOC
#define UP_CLIP_GPIO_PIN				GPIO_Pin_14
//�˵���
#define DOWN_CLIP_GPIO_PORT			GPIOC								
#define DOWN_CLIP_GPIO_CLK			RCC_APB2Periph_GPIOC
#define DOWN_CLIP_GPIO_PIN			GPIO_Pin_15

//�������
#define TOUCH_GPIO_PORT				GPIOB								
#define TOUCH_GPIO_CLK				RCC_APB2Periph_GPIOB
#define TOUCH_GPIO_PIN				GPIO_Pin_8
//������Դ����
#define TOUCH_POWER_GPIO_PORT			GPIOB								
#define TOUCH_POWER_GPIO_CLK			RCC_APB2Periph_GPIOB
#define TOUCH_POWER_GPIO_PIN			GPIO_Pin_9

#define TRIGGER_BUTTON 			PAin(4)	//�����ť
#define INSURANCE_BUTTON 		PAin(11)//���հ�ť
#define REST_BUTTON 				PBin(11)	//��λ��ť
#define CHAMBERING_BUTTON 	PCin(13)	//���Ű�ť
#define UP_CLIP_BUTTON 			PCin(14)	//�ϵ��а�ť
#define DOWN_CLIP_BUTTON 		PCin(15)	//�˵��а�ť
#define TOUCH_BUTTON 				PBin(8)	//������ť

#define TOUCH_POWER					PBout(9)//������Դ����

#define setbit(x,y) x|=(1<<y) //��X�ĵ�Yλ��1
#define clrbit(x,y) x&=!(1<<y) //��X�ĵ�Yλ��0

#define BULLET_NUMBER 12

void Ziyuan_GunState_Init(void);
void Ziyuan_Touch_Init(void);

void Gun_All_Status(void);
#endif
