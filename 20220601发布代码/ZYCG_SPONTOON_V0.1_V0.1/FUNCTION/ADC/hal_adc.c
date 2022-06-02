#include "hal_adc.h"


/************************************************************  
FileName: hal_adc.c   
Author: hf             
Date: 2021.03.23
Description: adc��ʼ�����ײ�����        
Version: V 1.0  
Other:
***********************************************************/ 


/**************************************************************   	
�������ƣ�	adc_init
�������ܣ�	adc��ʼ��
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
void Adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable ADC1 and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHZ 

	/* Configure PAx as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//�Ƿ���������ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//ADC���ݶ��뷽ʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//�涨��˳�����ת����ADCͨ����Ŀ
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
    ADC_TempSensorVrefintCmd(ENABLE);  //ʹ���ڲ��¶ȴ��������ڲ��ο���ѹ

	//�趨����ͨ��ת��ʱ��
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_13Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_13Cycles5);
	ADC_Cmd(ADC1, ENABLE);

	/* ����ADC1��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/* ���ADC1��λУ׼�Ĵ�����ĩ�� */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* ��ʼADC1У׼ */
	ADC_StartCalibration(ADC1);
	/* ���ADC1У׼���� */
	while(ADC_GetCalibrationStatus(ADC1));
}

void Adc_ShutDown(void)
{
	ADC_Cmd(ADC1, DISABLE);
}

//static void Adc_Start(void)
//{
//	ADC_Cmd(ADC1, ENABLE);
//}

/**************************************************************   	
�������ƣ�	Get_Adc
�������ܣ�	���ADCֵ
��ڲ�����	ch:ͨ��ֵ 0~3
���ز�����	��
˵����	    ��
**************************************************************/ 
static u16 Get_Adc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

//static float Calculate_AdcToVoltage(void)
//{
//	u16 adc;
//	adc=Get_Adc(ADC_Channel_1);
//	return (float)adc* 2 * (1.2/Get_Adc(ADC_Channel_17));  //(6 = ��׼��ѹ * 2)
//}

/**************************************************************   	
�������ƣ�	Get_Adc_Average
�������ܣ�	����ADCֵ��ƽ��ֵ
��ڲ�����	ch		:	ͨ��ֵ 0~3
						times	��ת������
���ز�����	��
˵����	    ��
**************************************************************/ 
u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val=0;
	
	Get_Adc(ch);
	for(u8 i=0; i<times; i++) {
		temp_val += Get_Adc(ch);
//		delay_ms(5);
	}
	return temp_val/times;
} 

//float calculate_bat(void)
//{
//	u16 adc;
//	adc=Get_Adc_Average(ADC_Channel_1,SAMPE_TIMES);
//	return (float)adc*6/4096;  //(6 = ��׼��ѹ * 2)
//}

/**************************************************************   	
�������ƣ�	calculate_bat
�������ܣ�	��ѹ����
��ڲ�����	��
���ز�����	��
˵����	    ��
**************************************************************/ 
float calculate_bat(void)
{
	float adcnum;
	u16 adc;
	
	adc=Get_Adc_Average(ADC_Channel_1,SAMPE_TIMES);
	adcnum = (float)adc*(3.3/4096);
	return (float)adc*(1.2/Get_Adc_Average(ADC_Channel_17,SAMPE_TIMES));  //(6 = ��׼��ѹ * 2)
}
