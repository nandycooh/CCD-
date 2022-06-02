#ifndef __USR_C322_H
#define __USR_C322_H	

#include "stm32f10x.h"
#include "power.h"
#include "usr_c322.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>
#include "protocol.h"
#include "md5.h" 
//#include "usart.h"
#include "motor.h"
#include "hal_flash.h"
#include "drv_SI4438.h"
#include "usart.h"	

/*********************************�궨�幦����**************************/
/*--------------------------------WIFIģ���������---------------------------------------------*/
#define CLIENT_PORT               8087//�ͷ��˵�port
#define SERVER_PORT               "8087"//��������port
#define BOARD_CAST_CYCLE        1000//�㲥�ļ��ʱ��
#define RX_NUM_SIZE 44

//STM32 ��Ƭ��
#define 	RestartSystem()								{	flash_config.reBoot.re_boot_flag = 1;\
																					GFLASH.Write(0,(u8*)&flash_config,sizeof(flash_config));\
																					delay_ms(100);\
																					__disable_fault_irq(); NVIC_SystemReset();\
																				}  /*�������ϵͳ*/

//�㲥����
__packed typedef struct
{
	u8 	MAC[6];
	u8 	IP[4];
	u16 port;
}BroadCastData_t;


//��������
__packed typedef struct
{
	u8 hash[12];//�豸���
	u8 pwd[16];//����-MD5
	u8 IP[4];//IP
}LoginOrBoardCastAckData_t;

//���ܱ�־λ
__packed typedef struct
{
	u8 IsWorkMode;//����ģʽ
	u8 IsLoginOrBoardCastAck;//��¼Ӧ���־
	u8 IsResume;
	u8 reConnectFlag;//���ӱ�־
	u8 IsconnectStatus;//�㲥�����־
	u8 IsUploadVersion;//�汾
	u8 IsUpBatteryFlag;//�����ϴ���־λ
	bool IsReoilFlag;//��������־λ
	u8 IsMotorFlag;
	bool IsSetChannel;
}funtionFlag_t;

/*********************************����������****************************/
void RS232_Init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void DataCallBack_WIFI(uint8_t* Data,uint16_t Len);
void Get_Mcu_Mac(void);
u8 Login_Or_Boardcast(uint8_t BoardcastMode);
void HeartCheckCallBack(void);
u8* GetVerison(void);
/*********************************API�ӿڲ�****************************/


#endif
