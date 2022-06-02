#include "attitude.h"


/*�����ϴ�*/
DataPayload_t dataPayload;

/*ȫ����̬����*/
attitude_data_t attitude_data;

static u16 gunstat,lwalkstat,rwalkstat,beltstat,spontoonstat,shieldstat,papperspraystat;//�ӻ�����״̬
static u8 handgun_online_flg,walkl_online_flg,walkr_online_flg,belt_online_flag,spontoon_online_flg,shield_online_flg,papperspray_online_flg;//�ӻ����߱�־
static u8 handgun_offline_flg,walkl_offline_flg,walkr_offline_flg,belt_offline_flag,spontoon_offline_flg,shield_offline_flg,papperspray_offline_flg;//�ӻ����߱�־

/*���� �� �� ��� �ҽ� ����*/
u8 gamePadBatteryData[5];
extern SI4438DataDef SI4438DataStruct;
/**
  * @brief ��ȡ��̬���� �첽��ȡ
  * @param  
  * @retval
**/
attitude_data_t* APL_attitudeGet(void)
{
	//////////////////////////////////////////////// ��� ////////////////////////////////////////////////////
	/*��Ŵ���״̬*/
	if(SI4438DataStruct.leftwalkflg==true)
	{
		walkl_online_flg++;
		walkl_offline_flg=0;
		
		if(walkl_online_flg>=3)
		{
			SI4438DataStruct.leftwalkflg=false;
			lwalkstat |= 0x0008;
		}	
	}
	else/*�������״̬*/
	{
		walkl_offline_flg++;
		walkl_online_flg=0;
		
		if(walkl_offline_flg>=33)//����2S����
		{
			lwalkstat &= 0x0000;
			memset(attitude_data.LeftWalk,0,sizeof(attitude_data.LeftWalk));
		}	
	}
	
	//////////////////////////////////////////////// �ҽ� ////////////////////////////////////////////////////
	/*�ҽ�����״̬*/	
	if(SI4438DataStruct.righttwalkflg==true)
	{
		walkr_online_flg++;
		walkr_offline_flg=0;
		
		if(walkr_online_flg>=3)
		{
				SI4438DataStruct.righttwalkflg=false;
				rwalkstat |= 0x0004;
		}	
	}
	else/*�ҽ�����״̬*/
	{
		walkr_offline_flg++;
		walkr_online_flg=0;
		
		if(walkr_offline_flg>=33)//����2S����
		{
			rwalkstat &= 0x0000;
			memset(attitude_data.RightWalk,0,sizeof(attitude_data.RightWalk));
		}	
	}
	
	///////////////////////////////////////////////// ��ǹ ////////////////////////////////////////////////////
	/*��ǹ����״̬*/
	if(SI4438DataStruct.gunflg==true)
	{
		handgun_offline_flg=0;
		handgun_online_flg++;
		
		if(handgun_online_flg>=3)
		{
			SI4438DataStruct.gunflg=false;
			gunstat |= 0x0010;
		}
	}
	else/*��ǹ����״̬*/
	{
		handgun_offline_flg++;
		handgun_online_flg=0;
		if(handgun_offline_flg>=33)//����2S����
		{					
			gunstat &= 0x0000;
//			memset(dataPayload.key_data,0,sizeof(dataPayload.key_data));
		}	
	}
	
	////////////////////////////////////////////////// ���� ////////////////////////////////////////////////////
	/*��������״̬*/
	if(SI4438DataStruct.beltflg==true)
	{
		belt_online_flag++;
		belt_offline_flag=0;
		
		if(belt_online_flag>=3)
		{
			SI4438DataStruct.beltflg=false;
			beltstat |= 0x0400;
		}
	}
	else/*��������״̬*/
	{
		belt_offline_flag++;
		belt_online_flag=0;
		
		if(belt_offline_flag>=33)//����2S����
		{
			beltstat &= 0x0000;//�Ͽ�״̬λ�������״̬������
			memset(attitude_data.QuaternionsData,0,sizeof(attitude_data.QuaternionsData));
		}	
	}
	
	///////////////////////////////////////////////// ���� ////////////////////////////////////////////////////
	/*��������״̬*/
	if(SI4438DataStruct.spontoonflg==true)
	{
		spontoon_offline_flg=0;
		spontoon_online_flg++;
		
		if(spontoon_online_flg>=3)
		{
			SI4438DataStruct.spontoonflg=false;
			spontoonstat |= 0x0002;
		}
	}
	else/*����״̬*/
	{
		spontoon_offline_flg++;
		spontoon_online_flg=0;
		if(spontoon_offline_flg>=33)//����2S����
		{					
			spontoonstat &= 0x0000;
//			memset(dataPayload.key_data,0,sizeof(dataPayload.key_data));
		}	
	}
	
	///////////////////////////////////////////////// ���� ////////////////////////////////////////////////////
	/*��������״̬*/
	if(SI4438DataStruct.shieldflg==true)
	{
		shield_offline_flg=0;
		shield_online_flg++;
		
		if(shield_online_flg>=3)
		{
			SI4438DataStruct.shieldflg=false;
			shieldstat |= 0x0040;
		}
	}
	else/*����״̬*/
	{
		shield_offline_flg++;
		shield_online_flg=0;
		if(shield_offline_flg>=33)//����2S����
		{					
			shieldstat &= 0x0000;
//			memset(dataPayload.key_data,0,sizeof(dataPayload.key_data));
		}	
	}
	
	///////////////////////////////////////////////// ����ˮ ////////////////////////////////////////////////////
	/*����ˮ����״̬*/
	if(SI4438DataStruct.pappersprayflg==true)
	{
		papperspray_offline_flg=0;
		papperspray_online_flg++;
		
		if(papperspray_online_flg>=3)
		{
			SI4438DataStruct.pappersprayflg=false;
			papperspraystat |= 0x0001;
		}
	}
	else/*����״̬*/
	{
		papperspray_offline_flg++;
		papperspray_online_flg=0;
		if(papperspray_offline_flg>=33)//����2S����
		{					
			papperspraystat &= 0x0000;
//			memset(dataPayload.key_data,0,sizeof(dataPayload.key_data));
		}	
	}
	
	attitude_data.OnLineState=lwalkstat | rwalkstat | gunstat | beltstat | spontoonstat | shieldstat | papperspraystat;//�ӻ�����״̬
	
	return &attitude_data;
}

/**
  * @brief  ��̬���ݴ����ϴ�ÿһ��ʱ���ϴ�һ��
  * @param  
  * @retval
**/
void attitudeUploadHandler(void)
{		
	u16 attitudeDataLen = sizeof(attitude_data_t);
//	dataPayload.Auto_Shutdown_Flg=9;
	/*��̬���� 210114 ȫΪ0*/
	memset(dataPayload.attitude_data,0,attitudeDataLen);
	
	memcpy(dataPayload.attitude_data,APL_attitudeGet(),attitudeDataLen);
	
	/*������̬���ݵ�PC ��̬���ݼ�6���ֽ��ֱ����� + 1���ֽڵ�����ģ��ͨ������*/
	protocol_sendData((uint8_t*)&dataPayload,attitudeDataLen+GAME_PAD_KEY_LEN+2,ATTITUDE_DATA_CMD);
}

/**
	* @brief ��صĵ����ϴ�
  * @param  
  * @retval
**/
u8* GetbatteryPresent(void)
{
	return gamePadBatteryData;
}

