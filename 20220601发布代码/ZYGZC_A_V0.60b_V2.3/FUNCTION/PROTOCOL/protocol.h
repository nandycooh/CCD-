#ifndef __PROTOL_H
#define __PROTOL_H

#include "stm32f10x.h"
#include "drv_SI4438.h"
#include "stdio.h"
#include "string.h"
#include "usr_c322.h"
#include "md5.h"  


#define MCU_TO_PC_BUF_SIZE        128//��λ���ϴ�����BUF�Ĵ�С
#define VERSION                   0x09//Э��汾�� ��2λ��汾��,��6λС�汾��:��(0.1.0)->(00 001 000)->hex(0x08)
#define ATTITUDE_DATA_CMD         0x03//��̬��������
#define BAT_DATA_CMD        			0x0A//����
#define VERSION_DATA_CMD        	0x0C//�汾�ϴ�
#define KEY_DATA_CMD        		  0x04//��������
#define BROADCAST_DATA_CMD        0x02//�㲥��������
#define LOGIN_DATA_CMD        		0x06//��¼��������
#define BOOT_OFF_DELAY         		20//�����ػ���ʱʱ��
#define POWER_KEY                 MACHINE_KEY2//��Դ����
#define BAT_UPDATA_TIME           3*20*3*20//�����ϴ����ٶ�1min���


#define USB_MODE_EN               0//usb vcpģʽ
#define CABLE_DEBUG_LOSS_EN       0//�Ƿ��ӡ���ߴ������Ķ�����
#define IS_4_POINT_MODE           1//�Ƿ����ó�4����Ԫ��ģʽ
#define IS_3_POINT_MODE           0//�Ƿ����ó�3����Ԫ��ģʽ
#define NEW_PROTOCOL_EN           1//�µ�Э��ʹ��MAC��ַ�̶��ϴ�
#define UCOSII_ENABLE        			1//�Ƿ�ʹ��ucos ii ����ϵͳ
#define IS_HEART_CHECK            1//�Ƿ���������� ����Ƿ�Ͽ�
#define LED_STATE_DEBUG_MODE      0//LED����ģʽʱ��״̬����ʽģʽ��״̬�л�
#define IS_SUPPORT_GUN            0//�Ƿ�֧��ǹ������
#define IS_LED_LOW_ENABLE         0//led�Ƿ��ǵ͵�ƽ��
#define IS_TEST_ADDR              0//�Ƿ�RF���ò��Եĵ�ַ����Ҫ���

#define MCU_TO_PC_BUF_SIZE        128//��λ���ϴ�����BUF�Ĵ�С
#define VERSION                   0x09//Э��汾�� ��2λ��汾��,��6λС�汾��:��(0.1.0)->(00 001 000)->hex(0x08)
#define ATTITUDE_DATA_CMD         0x03//��̬��������
#define BAT_DATA_CMD        			0x0A//����
#define VERSION_DATA_CMD        	0x0C//�汾�ϴ�
#define KEY_DATA_CMD        		  0x04//��������
#define BROADCAST_DATA_CMD        0x02//�㲥��������
#define LOGIN_DATA_CMD        		0x06//��¼��������
#define BOOT_OFF_DELAY         		20//�����ػ���ʱʱ��
#define POWER_KEY                 MACHINE_KEY2//��Դ����
#define BAT_UPDATA_TIME           3*20*3*20//�����ϴ����ٶ�20ms���

/*----------------------�汾��---------------------------------------------------------*/
/*ÿһ�ֽ���һ���汾��λ �ߣ���汾��,�У��а汾�ţ�С��С�汾�ţ�
���1���ֽ�����ǰ4λ��ʶ�豸��ʶ���� ABCDEFG ��Ӧ1234567 ���磺2x��ʶΪB�ˣ�7x��ʶΪG�� 
��4λ��ʶ�Ƿ�Ϊ���԰汾 x0���԰汾�� x1�����汾�� xF�����汾*/
#define HARDWARE_VERSION					0x0006007F//Ӳ���汾�� 5.2.0
#define SOFEWARE_VERSION					0x0002027F//����汾�� 5.2.1
#define PROTOCOL_VERSION          			0x0001037F//Э��汾�� 0.1.2 

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define LEFTWALKID		0x55
#define RIGHTWALKID		0x56
#define HANDGUNID		0x57
#define BELTID 			0x58
#define SPONTOON		0x59
#define SHIELD			0x60
#define PAPPER_SPRAY	0x61

///////////////////////////////////////////////////////////////////
//210611
//////////////////////////////////////////////////////////////////
#define MAX_R_BUF 28

/*���ݻ�����*/
typedef struct
{
	int Gun_Hardware_Version;
	int Gun_Soft_Version;
	int Gun_Protocol_Version;
	
	int Foot_Hardware_Version;
	int Foot_Soft_Version;
	int Foot_Protocol_Version;
	
	int Back_Hardware_Version;
	int Back_Soft_Version;
	int Back_Protocol_Version;
	
}SLAVE_VERSION_MSG;

/*���ݻ�����*/
typedef struct{
	uint8_t ID;
	uint8_t data[MAX_R_BUF]; //_attribute_((aligned(4)));
	uint16_t length;
}Msg_t;
/*433Э��*/
typedef struct
{
	uint8_t data[16*3];
}Protocol433_t;



//433�������ݱ�־λ
typedef struct
{
	bool gunflg;
	bool leftwalkflg;
	bool righttwalkflg;
	bool beltflg;
	bool spontoonflg;
	bool shieldflg;
	bool pappersprayflg;
}SI4438DataDef;



__packed typedef struct
{
	u8 	MAC[6];
}MacDataDef ;

void Si4438_Rcecv_Buff(void);
	

void SendHeartRateData(void);
///////////////////////////////////////
/**
  * @brief  �������ݵ�PC�� С��ģʽ
  * @param 
  * @retval
**/
u32 protocol_sendData(void * pData,u32 DataLen,u8 cmd);

/**
  * @brief  �ط�����
  * @param 
  * @retval
**/
void protocol_ResendData(u32 PackageCnt,void * pData,u32 DataLen,u8 cmd);

/**
  * @brief �ۼӺ�У�鷽ʽ
  * @param 
  * @retval
**/
u8 sumCheck(u8 *buf, u8 len);


/**
  * @brief crc8У��
  * @param 
  * @retval
**/
u8 crc8_maxim(u8 *data, u16 length);

u16 Message_count_CRC_2(u8 CrcLenth,u8 *CRC_Data);
void Ack_data_protocol(u8 slaveid,void * pData,u32 DataLen);
void SendMasterCmdData(void);
void Si433_Rcecv_Buff(void);
void Send_Master_Cmd_Data(u8 sendnum);
#endif

