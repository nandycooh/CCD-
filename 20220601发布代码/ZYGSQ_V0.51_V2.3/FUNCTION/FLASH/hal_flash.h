/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#ifndef __HAL_FLASH_H_
#define __HAL_FLASH_H_

#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "delay.h"

/*------------------------------------------------------------------------------------*/
//�����������
__packed typedef struct
{
	uint8_t 	device_num;
	uint8_t   pair_ch;//���ͨ��
	uint8_t  _XX2;
	uint8_t  _XX3;
}rf_pair_device_t;

//������������������
__packed typedef struct
{
	u8  ip[4];
	char  port[7];//��������ַ�
	u8 xx;
}serverConfig_t;

//������������������
__packed typedef struct
{
	u32 re_boot_flag;
	
}reBoot_t;

//wifi �������ݱ���
//������������������
__packed typedef struct
{
	u32 bund;
}offonline_wifi_t;

//���߱���
typedef struct
{
	serverConfig_t servercofig;
	
	rf_pair_device_t pair_device;
	
	reBoot_t reBoot;
	
	offonline_wifi_t offonline_wifi;
	
}flash_config_t;

//flash������
typedef struct
{
	/**
		* @brief  д������
		* @param  addr д��ĵ�ַ ,d д������� lenд������ݵĳ���
		* @retval : int ���ض������ݵĴ�С
	**/
	int (*Read)(uint32_t addr,u8 *d,uint16_t len);
	/**
		* @brief  ����Ϊ�������
		* @param  channel io�� ,out_input �����������
		* @retval : None
	**/
	void (*Write)(uint32_t addr,u8 *d,uint16_t len);
	
}HalAdapter_Management_FLASH_t;

/*���߲���*/

/* Private Values -------------------------------------------------------------*/
/****************************************************************
*Name:		ReadFlashNBtye
*Function:	���ڲ�Flash��ȡN�ֽ�����
*Input:		ReadAddress�����ݵ�ַ��ƫ�Ƶ�ַ��ReadBuf������ָ��	ReadNum����ȡ�ֽ���
*Output:  ��ȡ���ֽ���  
*Author:    ValerianFan
*Date:		2014/04/09
*E-Mail:	fanwenjingnihao@163.com
*Other:		
****************************************************************/
int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, uint16_t ReadNum) ;

/****************************************************************
*Name:		WriteFlashOneWord
*Function:	���ڲ�Flashд��32λ����
*Input:		WriteAddress�����ݵ�ַ��ƫ�Ƶ�ַ��WriteData��д������
*Output:	NULL 
*Author:    ValerianFan
*Date:		2014/04/09
*E-Mail:	fanwenjingnihao@163.com
*Other:		
****************************************************************/
void WriteFlashOneWord(uint32_t WriteAddress,uint8_t* Buf,uint16_t size);


void FLASH_Read_MoreData(uint32_t startAddress,uint16_t *readData,uint16_t countToRead);
void FLASH_WriteMoreData(uint32_t startAddress,uint16_t *writeData,uint16_t countToWrite);
#endif
