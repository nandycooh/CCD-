#ifndef __ATTITUDE_H
#define __ATTITUDE_H	

#include "sys.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>
#include "usr_c322.h"
#include "drv_SI4438.h"

/* Macro -------------------����ȫ������-----------------------------------------------------*/

#if IS_3_POINT_MODE > 0
	#define   CABLE_SLAVE_DEVICE_8_EN         0
#else
	#define   CABLE_SLAVE_DEVICE_8_EN         1
#endif

#if CABLE_SLAVE_DEVICE_8_EN > 0
	#define CABLE_SLAVE_DEVICE_CNT    				10//������̬���������� 6�� ����8�� ����С��6��
#else
	#define CABLE_SLAVE_DEVICE_CNT    				6//������̬���������� 6�� ����8�� ����С��6��
#endif

#define GAMEPAD_SENSOR_PROTOCOL_HEAD_LEN    3//�ֱ�������Э��ͷ���ݳ���
#define WIRED_SENSOR_PROTOCOL_HEAD_LEN      3//���ߴ�����Э�����ݳ���
#define WIRED_EULER_PROTOCOL_OFFEST					8//Э��ͷ+��Ԫ��
#define WIRELESS_KEY_DATA_OFFSET \
	GAMEPAD_SENSOR_PROTOCOL_HEAD_LEN + \
	QUATERNIONS_DATA_LEN+1                    //��������ƫ��
#define ROCKER_DATA_OFFSET \
	WIRELESS_KEY_DATA_OFFSET+1                //ҡ������ƫ��

#define CABLE_SENSOR_DATA_LEN     					11//���ߴ�������Ч���ݳ���
#define CABLE_WAIST_SENSOR_DATA_LEN     		17//���ߴ�������Ч���ݳ���
#define WIRELESS_SLAVE_DEVICE_CNT 					4//���ߴ������ĸ��� ���������ֱ�  �����Ų�������
#define SENSOR_DATA_LEN                     (QUATERNIONS_DATA_LEN+1+1)//1���ֽھ���1���ֽ�����״̬
#define ALL_SENSOR_QUATERNIONS_DATA_LEN   	(QUATERNIONS_DATA_LEN*SENSOR_NUM)//�ܵ���Ԫ���ݳ���
#define SENSOR_NUM													14//�ܵĴ��������ĸ���û�еĴ�����ռλ
#define GAME_PAD_KEY_LEN                  	10//�ֱ����ݰ������ݵĳ���
#define GUN_485_ADDR                        0x11//ǹ�ĵ�ַ
#define WAIST_485_ADDR											0x05//���ĵ�ַ
#define QUATERNIONS_DATA_LEN      					(4)//��Ԫ�������ݳ��� ��16λ
#define EULER_DATA_LEN											(6)//ŷ�������ݳ���
#define GAMEPAD_DATA_LEN          					(12)//�ֱ����ݳ���
#define FOOTSTEP_DATA_LEN         					(14)//�Ų����������ݳ���
/****�Ų����ж���****/
#define WALK_PROTOCOL_OFFEST								7//Э��ͷ+��Ԫ��
#define WALK_DATA_LEN												3
/*-----------------*/

__packed typedef struct
{
	/*ȫ����̬���ݣ��������ϰ���������Ԫ����
	�ݺ������˶�״̬������ɣ���Ԫ��Ϊ��һ����
	������ݣ���ΪС��������100�����ϴ����ϴ�
	˳��ΪW,X,Y,Z,������������ϵ����32�ֽ��з�
	������*/
	u8 QuaternionsData[ALL_SENSOR_QUATERNIONS_DATA_LEN];
	
	/*����ŷ��������X��Y��Z ÿ����ռ2λ����Χ
	0-360��*/
	u8 EulerData[EULER_DATA_LEN];
	
	/*�Ų���������*/
	u8 LeftWalk[WALK_DATA_LEN];
	u8 RightWalk[WALK_DATA_LEN];
	
	/*�����˶�����״̬��Ϊ�޷������ݣ���1�ֽڣ�
	��ЧΪ1����λΪ0������Ϊ�˵������������㣬
	���ң����ͬ����ǰ��־λ��ʱ����*/
	u8 MotionState;
	
	/*ȫ����̬��Ӧ��״̬���޷������ݣ���2�ֽڣ�
	ÿһλ��ʾһ����λ�Ĵ��������ȣ�1��ʾ����
	���ľ��ȴﵽ��3������߾��ȣ���0��ʾ������
	�ľ���Ϊ��3. bit5~bit0 ����λ*/
	u16 SensorAccuracy;
	
	/*ȫ����̬����״̬:�޷������ݹ�2�ֽ�
	ÿһλ��ʾһ����λ�Ĵ�����״̬,1��ʾ����
	����״̬������,0��ʾ��������״̬Ϊ����*/
	u16 OnLineState;
	
}attitude_data_t;

//�����ϴ�
__packed typedef struct
{
	/*��̬���ݺͰ��������ϴ��̶������ֽ�*/
	u8 attitude_data[sizeof(attitude_data_t)];
	
	/*GAME_PAD_KEY_LEN���ֽڵİ�������*/
	u8 key_data[GAME_PAD_KEY_LEN];
	
	/*����һ���ֽڵ�433�ŵ���ʾ*/
	u8 nrf_channel;
	
	u8 Auto_Shutdown_Flg;
	/*�㲥�����ϴ�*/
//	BroadCastData_t BroadCastData;//��Э�鲻���㲥����
	
}DataPayload_t;

u8* GetbatteryPresent(void);	
void attitudeUploadHandler(void);
	
#endif
