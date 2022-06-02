#include "emserial.h"


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{  
    void SendOneByte(u8 Byte);
	SendOneByte((uint8_t) ch);    
	return ch;
}
#endif 

/**
  * @brief TIM3����
  * @param  None
  * @retval : None
**/
static void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);    

}

/*********************************Ӳ��������****************************/
void Emserial_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(EMULATE_SERIAL_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(EMULATE_SERIAL_PORT, EMULATE_SERIAL_PIN);           //����Ĭ��λΪ�ߵ�ƽ
	TIM3_Configuration();
}

void SerialDelay_Us(__IO uint32_t nTime)
{ 

	uint16_t tmp;

	tmp = TIM_GetCounter(TIM3);
	
	if(tmp + nTime <= 65535)
		while( (TIM_GetCounter(TIM3) - tmp) < nTime );
	else
	{
		TIM_SetCounter(TIM3, 0);
		while( TIM_GetCounter(TIM3) < nTime );
	}	
}

//ģ�����ŷ��͵����ֽ�
void SendOneByte(u8 Byte)
{
  u8 i = 0;
  EMULATE_SERIAL_IO=0;             //�������ͣ�������������
  SerialDelay_Us(BuadRate115200);  //��ʱ104us
  for(i = 0; i < 8; i++) //8λ����λ
  {
      if(Byte & 0x01)    //���bitλΪ1
          EMULATE_SERIAL_IO=1;  //��������
      else    //��������
          EMULATE_SERIAL_IO=0;
      SerialDelay_Us(BuadRate115200);
      Byte >>= 1;
  }
  EMULATE_SERIAL_IO=1;   //ֹͣλ
  SerialDelay_Us(BuadRate115200);
	delay_ms(1);//��ֹ����210723
}

void SendBytes(u8 *str,u8 len)	//������������������Ҳ�ɷ����ַ���
{
  u16 i;
  for(i=0;i<len;i++)
  {
  	SendOneByte(*str);
  }
}

void SendStr(u8 *str) //�����ַ������������������0x00���жϷ�����
{
 while(*str)
 SendOneByte(*str++);

}
