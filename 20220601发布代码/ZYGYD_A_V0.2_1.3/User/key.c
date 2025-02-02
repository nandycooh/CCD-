#include "key.h"
#include "stm32f10x_gpio.h"

void init_key(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //input pullup
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void init_backKey(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //input pullup
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void delay_ms(uint32_t dly) 
{
	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

u8 key_scan(void)
{
	u8 key_state = 0;

	if(KEY0==1) {
		delay_ms(10);
		if(KEY0==1) {
			key_state |= 0x01;
		}
	}

	if(KEY1==1) {
		delay_ms(10);
		if(KEY1==1) {
			key_state |= 0x02;
		}
	}

	if(KEY2==1) {
		delay_ms(10);
		if(KEY2==1) {
			key_state |= 0x04;
		}
	}

	if(KEY3==1) {
		delay_ms(10);
		if(KEY3==1) {
			key_state |= 0x08;
		}
	}

	if(KEY4==1) {
		delay_ms(10);
		if(KEY4==1) {
			key_state |= 0x10;
		}
	}
	if(KEY5==1) {
		delay_ms(10);
		if(KEY5==1) {
			key_state |= 0x20;
		}
	}

	if(KEY6==1) {
		delay_ms(10);
		if(KEY6==1) {
			key_state |= 0x40;
		}
	}
	return key_state;
}


