#ifndef __FUNC_CONFIG_H
#define __FUNC_CONFIG_H

#include "stm32f10x.h"
#include <stdint.h>
#include <stdbool.h>

/***************************TIMER******************************/
/*���ⶨʱ���ĸ���*/
#define TIME2_EVENT_NUM 							7
#define TIME4_EVENT_NUM 							9

/*��ʱ��ID�������ܳ�����Ӧ��ʱ������*/
#define RF_RECEIVE										1
#define UPDATE_BATTERY								2 //���оƬ״̬���¶�ʱ��
#define LED_FLICKER										3 //LED��˸��ʱ��

/*��ʱ��ʱ�䳤�̹���*/
#define UPDATE_BATTERY_TIME						200 //200ms����һ�ε�ص���

/***************************POWER******************************/
//#define BAT_CAPACITY_LV1   4.0      //��Ӧ��ʾ����80%
//#define BAT_CAPACITY_LV2   3.85     //��Ӧ��ʾ����60%
//#define BAT_CAPACITY_LV3   3.77     //��Ӧ��ʾ����40%
//#define BAT_CAPACITY_LV4   3.72     //��Ӧ��ʾ����20%
//#define BAT_CAPACITY_LV5   3.6      //��Ӧ��ʾ������
//#define BAT_CAPACITY_LV6   3.3      //��Ӧǿ�ƹػ�����

#define BATQUEUE_LENGTH						10//��ص�ѹ���г��ȣ����ڼ���ƽ����ѹֵ

/***************************MODULE*****************************/
//����ID����
#define POWER_MASK								0x00
#define RF_MASK 									0x01
#define LED_MASK									0x02
#define MPU_MASK									0x03
#define BATADC_MASK								0x04

/***************************LED********************************/
#define IS_LED_LOW_ENABLE         1//led�Ƿ��ǵ͵�ƽ���� ʹ�ܵ͵�ƽ��

#define PB13 							BIT_ADDR(GPIOB_ODR_Addr, 13)
#define PB14 							BIT_ADDR(GPIOB_ODR_Addr, 14)
#define PB15 							BIT_ADDR(GPIOB_ODR_Addr, 15)

#endif
