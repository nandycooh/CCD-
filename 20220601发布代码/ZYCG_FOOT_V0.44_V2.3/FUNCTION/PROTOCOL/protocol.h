#ifndef __PROTOL_H
#define __PROTOL_H

#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>

//���ݻ���buf��С
#define MAX_R_BUF 28
//���ݷ�װbuf��С
#define MSG_SEND_BUFFER          26//��Ч���ݵ���󳤶�

#define FORMAT_HEAD1 0x77
#define FORMAT_HEAD2 0x76
#define FORMAT_TAIL  0x75

#pragma pack(push)//�������״̬ 
#pragma pack(1)//�趨Ϊ1�ֽڶ��� 


/*----------------------�汾��---------------------------------------------------------*/
/*ÿһ�ֽ���һ���汾��λ �ߣ���汾��,�У��а汾�ţ�С��С�汾�ţ�
���1���ֽ�����ǰ4λ��ʶ�豸��ʶ���� ABCDEFG ��Ӧ1234567 ���磺2x��ʶΪB�ˣ�7x��ʶΪG�� 
��4λ��ʶ�Ƿ�Ϊ���԰汾 x0���԰汾�� x1�����汾�� xF�����汾*/
#define HARDWARE_VERSION					0x0004047F//Ӳ���汾�� 5.2.0
#define SOFEWARE_VERSION					0x0002037F//����汾�� 5.2.1
#define PROTOCOL_VERSION          			0x0001027F//Э��汾�� 0.1.2 

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

//Э��1�ֽڶ���
typedef struct {
	uint8_t 	MsgHead[2];
		uint8_t MsgPayLoad[MSG_SEND_BUFFER];//����Э�鶨������ʵ�������岻�������ü���׶�,��Ҫȱ�ٶ��������͵Ĺ����������ݵ�һ��
}MsgProtocol_t;

#pragma pack(pop)
//��ʽ��
typedef struct
{
	void(*FmtU8)(uint8_t d);
	void(*FmtU16)(uint16_t d);
	void(*FmtU32)(uint32_t d);
	void(*FmtStr)(char* str);
	void(*FmtHex)(uint8_t* Dt, uint16_t len);
	void(*FmtHead)(void);
	void(*Fmtend)(void);
//	void(*FmtCmd)(uint16_t cmd);
	void(*FmtCrc16)(void);
	void(*FmtCrc8)(void);
	uint8_t*  (*GetBuffer)(void);
	uint16_t(*GetLength)(void);
}ProtocolFormat_t;

ProtocolFormat_t* SendProtocolGetObj(void);
MsgProtocol_t*  Protocol_Analysis(void* d);


///////////////////////////////////////////////////////////
/*���ݻ����� ��������*/
typedef struct{
	uint8_t ID;
	uint8_t data[MAX_R_BUF];
	uint16_t length;
}Msg_t;

/*433Э�� Ӧ������*/
typedef struct
{
	uint8_t data[16];
}Protocol433_t;

extern u8 gun_statusflg;
void SendKeyDown(void);
void SendKeyUp(void);
void Handgun_Send433Data(void);
void Si4438_Rcecv_Buff(void);
void RF433_SendData(Msg_t *d);
void Handgun_SendData(void);
u8* GetVerison(void);
#endif

