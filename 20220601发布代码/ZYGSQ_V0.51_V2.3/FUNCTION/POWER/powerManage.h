#ifndef __POWERMANAGE_H
#define __POWERMANAGE_H

#include "stm32f10x.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"



#define BAT_CHARGE_PIN 	GPIO_Pin_12    //�ڲ���USB���жϣ�����У���   ��������
#define USB_CHECK_PIN 	GPIO_Pin_10
#define BAT_CHRG_IN     PBin(12)
#define USB_CHECK_IN    PBin(10)

#define READBAT  GPIO_ReadInputDataBit(GPIOB, BAT_CHARGE_PIN)//���ʱ��״̬
#define READUSB  GPIO_ReadInputDataBit(GPIOB, USB_CHECK_PIN)//�Ƿ����USB

#define BAT_CAPACITY_LV1   1.35     //��Ӧ��ʾ����80%
#define BAT_CAPACITY_LV2   1.30     //��Ӧ��ʾ����60%
#define BAT_CAPACITY_LV3   1.22    //��Ӧ��ʾ����40%
#define BAT_CAPACITY_LV4   1.15     //��Ӧ��ʾ����20%
#define BAT_CAPACITY_LV5   1.10     //��Ӧ��ʾ������
#define BAT_CAPACITY_LV6   1.06     //��Ӧǿ�ƹػ�����

void charge_IC_RQ_shutdown(void);
void charge_led_dsp(void);
void Enter_Sleep(void);

#endif
