#ifndef __EMSERIAL_H
#define __EMSERIAL_H


#include "delay.h"
#include "stdio.h"



#define EMULATE_SERIAL_PIN 		GPIO_Pin_12
#define EMULATE_SERIAL_PORT 		GPIOB
#define EMULATE_SERIAL_CLK 		RCC_APB2Periph_GPIOB

#define LED_OFF   0
#define LED_ON    1

#define EMULATE_SERIAL_IO   PBout(12)

//�Բ�����9600Ϊ��������һ��bitλ�ļ������1/9600s����Լ����104us����һ��bitλ��
// 25=38400 52=19200 16=57600 ���֧��571600 �ٴ�ͻ�������
#define BuadRate9600 104
#define BuadRate1200 830
#define BuadRate38400 25
#define BuadRate19200 52
#define BuadRate57600 16
#define BuadRate115200 8


void Emserial_Init(void);

#endif


