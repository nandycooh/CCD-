#include "../function/led/rgb_led.h"
#include "../function/spi/drv_spi.h"
#include "../function/time/hal_timer.h"
#include "../function/si4438/drv_SI4438.h"
#include "../function/power/powerManage.h"
#include "../function/peripheral/peripheral.h"
#include "../function/protocol/protocol.h"
#include "../function/recoil/recoil.h"
#include "../function/time/hal_timer.h"
#include "../function/crc/crc.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "hal_flash.h"

ProtocolFormat_t* pP_Instance;				//Э���
extern TIME_ControlDef TIME_ControlStruct;	//��ʱ����װ
extern Msg_t p_Msg;							//�������ݽṹ��
extern Protocol433_t *Protocol4438;			//�������ݽṹ��
u8 recoi_mode=0;
static u8 recoicmd=0;

u8 Si4438ChannelSet=0;
extern u32 time_count;
extern u8 recoil_stop;
extern u32 sleep_count;
extern u8 recoil_abnormal;//�������쳣����
extern u8 bullet_count;
extern uint8_t gun_statusflg[2];
extern uint8_t g_SI4463RxBuffer[64];	//�������ݻ���buffer

static u8 Version[12];
u8* GetVerison(void)
{
	int i = 0;
	int temp = HARDWARE_VERSION;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = BYTE0(temp);
	temp = SOFEWARE_VERSION;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = BYTE0(temp);
	temp = PROTOCOL_VERSION;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i] = 	 BYTE0(temp);
//	memcpy(&Version[0],&res[0],12);
	return Version;
}

/*��ǹЭ������*/
void Gun_Task(void){
	static uint8_t quat[4] = {0};
	static uint8_t quat1[6] = {0};
			/*�Զ���Э��*/
			pP_Instance->FmtHead();
			pP_Instance->FmtU8(HANDGUNID);								//�豸ID
			pP_Instance->FmtHex(quat,4);									//
			pP_Instance->FmtU8(gun_statusflg[0]);						//��ǹ״̬
			pP_Instance->FmtHex(quat1,6);
			
			pP_Instance->FmtHex(GetVerison(),12);//220602
			
			pP_Instance->FmtCrc8();												//У��λ
			pP_Instance->FmtU8(FORMAT_TAIL);							//����β֡
			p_Msg.length = pP_Instance->GetLength();
			memcpy(p_Msg.data,pP_Instance->GetBuffer(), p_Msg.length);
}

void recoi_E(void)
{
	if(!INSURANCE_BUTTON)
		TIM_Cmd(TIM3,ENABLE);
	else
		TIM_Cmd(TIM3,DISABLE);
}


static u8 lastcanncel;
extern HalAdapter_Management_FLASH_t GFLASH;
extern flash_config_t flash_config;
void RF_SI4438_Ack_Cmd_Data(void)
{
	u8 crcdata;
	
	if(Protocol4438->data[0] == 0x66 && Protocol4438->data[1] == 0x65 && Protocol4438->data[2] == HANDGUNID &&\
			Protocol4438->data[3] ==  Si4438ChannelSet && Protocol4438->data[15]==0x64)
	{
//		printf(" %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\r\n",Protocol4438->data[0],Protocol4438->data[1],Protocol4438->data[2],Protocol4438->data[3],
//			Protocol4438->data[4],Protocol4438->data[5],Protocol4438->data[6],Protocol4438->data[7],Protocol4438->data[8],Protocol4438->data[9],Protocol4438->data[10],Protocol4438->data[11],Protocol4438->data[12],
//			Protocol4438->data[13],Protocol4438->data[14],Protocol4438->data[15]);
		
			crcdata = crc8_maxim(Protocol4438->data,14);//���ߺ�У�鲻�������޷�����
//			printf("%x %x\r\n",Protocol4438->data[14],crcdata);
			if(Protocol4438->data[14] != crcdata)
			{
				memset(g_SI4463RxBuffer,0,sizeof(g_SI4463RxBuffer));
				return;
			}
			
			//�ŵ���Ϊ0���Ҳ������η��������ŵ�����ֹд��flash������
			if(Protocol4438->data[5]!=0 && lastcanncel != Protocol4438->data[5])
			{
				Si4438ChannelSet=Protocol4438->data[5];
				lastcanncel=Protocol4438->data[5];//������һ�ε��ŵ�
				flash_config.pair_device.pair_ch=Si4438ChannelSet;//�����ŵ���flash
				GFLASH.Write(0,(u8*)&flash_config,sizeof(flash_config));
				SI446x_Init();
				printf("GChannelSet=%d\r\n",Si4438ChannelSet);
			}
			
			if(recoicmd)
			{
				switch(Protocol4438->data[13])
				{
					case 0x03:
						LED_ON;
						if(recoil_stop==1)//�а�������Ų���������201111
						{
							recoil_stop=0;
							bullet_count=1;TIM_Cmd(TIM3,ENABLE);
						}
				break;
					default:
						LED_OFF;
					
						if(!bullet_count)//�ȴ���Ͳִ����� �Է���λ
						{
							TIM_Cmd(TIM3,DISABLE);time_count = 0;
						}break;
				}
			}
			
			RF433_SendData(&p_Msg);//��������
			
			memset(&Protocol4438,0,sizeof(Protocol4438));//��ս�������
		}
		else
			Si4438_Rcecv_Buff();//�������ݴ���
}

/**
  * @brief :������ 
  * @param :��
  * @note  :��
  * @retval:��
***/ 
int main( void )
{					
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init( );									//��ʱ��ʼ��
	uart_init( 115200 );							//����1��ʼ�� ����ʹ��
	Led_Init();										//LED��ʼ��
	Ziyuan_GunState_Init();							//ǹ��ʼ��
	Ziyuan_Touch_Init();							//������ʼ��
	Recoil_Init();									//���������ֳ�ʼ��
	
	GFLASH.Read(0,(u8*)&flash_config,sizeof(flash_config));//��ȡ��������
	if(flash_config.pair_device.pair_ch != 0xff)
		Si4438ChannelSet=flash_config.pair_device.pair_ch;
	else
		Si4438ChannelSet=0;
	printf("Si4438ChannelSet=%d\r\n",Si4438ChannelSet);
	
	//SI4438��ʼ��
	SI44xx_Power_Init();
	delay_ms(1000);//�ȴ�������
	SI446x_Init();				
	
	TIME_ControlStruct.start(1,1000);				//��ʼ����ʱ��2 1S 		����������ͨ�����
	TIME_ControlStruct.start(2,1);					//��ʼ����ʱ��4 1ms 	����������� ˯�߼���
	
	pP_Instance = SendProtocolGetObj();				//ʵ����Э���
	
	while(1)
	{
		Gun_All_Status();							//��ǹ����״̬��ȡ
		Gun_Task();									//Э���װ
		RF_SI4438_Ack_Cmd_Data();					//����Ӧ��
		
		if(!DOWN_CLIP_BUTTON && !REST_BUTTON)		//��ϼ�-����������ģʽ�л�
		{
			delay_ms(10);
			if(!DOWN_CLIP_BUTTON && !REST_BUTTON)
			{
				recoi_mode=(!recoi_mode);
				while(1)
				{
					LED_PIN_PORT=!LED_PIN_PORT;
					delay_ms(100);
					
					if(DOWN_CLIP_BUTTON && REST_BUTTON)
						break;
				}
			}				
		}
		
		if(recoi_mode)							//���������Ʒ�ʽ������ or SDKָ��
		{
			recoi_E();
			recoicmd=0;
		}
		else
			recoicmd=1;
			
		
		/*���������쳣�������*/
		if(CHAMBERING_BUTTON)						//��⵽����  �����������쳣����
		{
			TIM_Cmd(TIM2,ENABLE);
			if(recoil_abnormal>=2)
			{
				TIM1->CCER = 0;//���
				TIM_Cmd(TIM1,DISABLE);
			}	
		}
		else
		{
			TIM_Cmd(TIM2,DISABLE);
			recoil_abnormal=0;
		}
		
		/*���߲��ֹ���*/	
		if(sleep_count>10*1000*60)					//10�������ֳ��Զ����� �ֳ�ʱ�Զ�����
		{
			sleep_count=0;
			Enter_Sleep();
		}
	}
}



