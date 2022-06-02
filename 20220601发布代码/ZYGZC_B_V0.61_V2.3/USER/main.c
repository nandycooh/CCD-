#include "../function/led/led.h"
#include "../function/spi/hal_spi.h"
#include "../function/si4438/drv_SI4438.h"
#include "../function/protocol/protocol.h"
#include "delay.h"
#include "usart.h"
#include "power.h"
#include "timer1.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "USART2.h"
#include "main.h"
#include "../function/crc/crc.h"
#include "hal_flash.h"

#define TIM_arr 999
#define TIM_psc 71
#define LEVEL_0  320    //0����ǿ�ȣ���ֹ��0��ʱ���������

ProtocolFormat_t* pP_Instance;				//Э���

extern R485_t Recv_485_Msg;//�����������ݻ���
extern Protocol433_t *Protocol4438;
extern Msg_t p_Msg;

static u8 connectstas;
extern uint8_t g_SI4463RxBuffer[64];	//�������ݻ���buffer

u8 shakeData[54];
u8 shakeDataLen;
u8 Si4438ChannelSet=0;

//static u8 lastcanncel;
extern HalAdapter_Management_FLASH_t GFLASH;
extern flash_config_t flash_config;

/*
tim1:ch1ͷ��ǰ�ch2����  ch3���أ�  ch4����
tim2:ch1���ߣ�    ch2���ߣ�  ch3�󸹣�  ch4�Ҹ�
tim3:ch1�󱳣�    ch2�ұ���  ch3�������ch4�Һ���
tim4:ch1���ۣ�  ch2��С�ۣ�ch3���֣�  ch4�Ҵ��
tim5:ch1��С�ۣ�  ch2���֣�  ch3����ȣ�ch4��С��
tim8:ch1��ţ�    ch2�Ҵ��ȣ�ch3��С�ȣ�ch4�ҽ�
 */
struct shakeType
Head, Occiput, Chest_L, Chest_R, Rib_L, Rib_R, Belly_L, Belly_R,
Back_L, Back_R, Waist_L, Waist_R, Arm_L, Wrist_L, Hand_L, 
Arm_R, Wrist_R, Hand_R, Thigh_L, Shank_L, Foot_L, 
Thigh_R, Shank_R, Foot_R, Shoulder_L, Shoulder_R;

void shakeData_Analysis(void)
{
    u8 location;
    u8 NUM;
    
    NUM = shakeDataLen / 2;//���յ�Э��Ϊ2�ֽ� ��һ���ֽ�Ϊλ�ú�ǿ�� �ڶ����ֽ�Ϊʱ��211207
    for(u8 i = 0; i < NUM; i++)
    {
        location = shakeData[i * 2] & 0xF8;
        location = location >> 3;
		
		switch(location)
        {
#ifdef  SHOULDER
            case 1://���
                Shoulder_L.shakeStrength = shakeData[i * 2] & 0x07;//��ǿ��
                Shoulder_L.shakeTime = (shakeData[i * 2 + 1] & 0x1F);//����ʱ��
                Shoulder_L.shakeTime = Shoulder_L.shakeTime * 100;//ʵ�ʳ���ʱ��
                SHOULDER_L_TIM = Shoulder_L.shakeStrength * 60 + LEVEL_0;
                break;
            case 2://�Ҽ�
                Shoulder_R.shakeStrength = shakeData[i * 2] & 0x07;
                Shoulder_R.shakeTime = (shakeData[i * 2 + 1] & 0x1F);
                Shoulder_R.shakeTime = Shoulder_R.shakeTime * 100;
                SHOULDER_R_TIM = Shoulder_R.shakeStrength * 60 + LEVEL_0;
                break;
#endif
            
#ifdef CHEST
            case 3://����Chest_L
                Chest_L.shakeStrength = (shakeData[i * 2] & 0x07);  //���յ���λ���´�����ǿ����ֵ
                Chest_L.shakeTime = (shakeData[i * 2 + 1] & 0x1F);  //���յ���λ���´�����ʱ����ֵ
                Chest_L.shakeTime = Chest_L.shakeTime * 100;   //��ʱ������100������λms
                CHEST_L_TIM = Chest_L.shakeStrength * 60 + LEVEL_0; //��ǿ������60���ټ���520
                                                                //����520��Ϊ�˱�����ǿ�ȹ�С��û����Ч�������޸�520
                break;
            case 4://����
                Chest_R.shakeStrength = (shakeData[i * 2] & 0x07);
                Chest_R.shakeTime = (shakeData[i * 2 + 1] & 0x1F);
                Chest_R.shakeTime = Chest_R.shakeTime * 100;
                CHEST_R_TIM = Chest_R.shakeStrength * 60 + LEVEL_0;
                break;
#endif
#ifdef BACK
            case 9://��Back_L
                Back_L.shakeStrength = (shakeData[i * 2] & 0x07);
                Back_L.shakeTime = (shakeData[i * 2 + 1] & 0x1F);
                Back_L.shakeTime = Back_L.shakeTime * 100;
                BACK_L_TIM = Back_L.shakeStrength * 60 + LEVEL_0;
                break;
#endif
            default:
                break;
        }   
    }
}

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

/**************************************************************************/
void SI443x_dataHandle(void)
{
    shakeDataLen = Protocol4438->data[8];//���ݳ���
    memcpy(shakeData, &Protocol4438->data[10], shakeDataLen);
}

/*��ǹЭ������*/
void Gun_Task(void){
			/*�Զ���Э��*/
			pP_Instance->FmtHead();
			pP_Instance->FmtU8(BELTID);								//�豸ID
			pP_Instance->FmtHex(Recv_485_Msg.belt,4);					//��������
			pP_Instance->FmtU8(Recv_485_Msg.back);						//����״̬ 1-����
			pP_Instance->FmtHex(Recv_485_Msg.data,6);					//����xyz
			
			pP_Instance->FmtHex(GetVerison(),12);//220602
	
			pP_Instance->FmtCrc8();												//У��λ
			pP_Instance->FmtU8(FORMAT_TAIL);							//����β֡
			p_Msg.length = pP_Instance->GetLength();
			memcpy(p_Msg.data,pP_Instance->GetBuffer(), p_Msg.length);
	
//			printf("%x %x %x %x-%x-%x %x %x %x %x %x\r\n",Recv_485_Msg.belt[0],Recv_485_Msg.belt[1],Recv_485_Msg.belt[2],Recv_485_Msg.belt[3],Recv_485_Msg.back,
//					Recv_485_Msg.data[0],Recv_485_Msg.data[1],Recv_485_Msg.data[2],\
//					Recv_485_Msg.data[3],Recv_485_Msg.data[4],Recv_485_Msg.data[5]);
}

void RF_SI4438_Ack_Cmd_Data(void)
{
	static u16 Reoil_Cont;
	u8 crcdata;

	if(Protocol4438->data[0] == 0x66 && Protocol4438->data[1] == 0x65 && Protocol4438->data[2] == BELTID &&\
			Protocol4438->data[3] ==  Si4438ChannelSet && Protocol4438->data[15]==0x64)
	{
		crcdata = crc8_maxim(Protocol4438->data,14);
		
		if(Protocol4438->data[14] != crcdata)
		{
			memset(g_SI4463RxBuffer,0,sizeof(g_SI4463RxBuffer));
			return;
		}
		
		if(Protocol4438->data[7]!=0)
		{
			Si4438ChannelSet=Protocol4438->data[7];
			
			flash_config.pair_device.pair_ch=Si4438ChannelSet;//�����ŵ���flash
			GFLASH.Write(0,(u8*)&flash_config,sizeof(flash_config));
			SI446x_Init();
			printf("GChannelSet=%d\r\n",Si4438ChannelSet);
		}
			
		Reoil_Cont=0;
		connectstas=1;
		

		RF433_SendData(&p_Msg);//��������
		memset(p_Msg.data,0,sizeof(p_Msg.data));//��շ�������
		p_Msg.ID=0x00;
		memset(Protocol4438,0,sizeof(Protocol433_t));//��ս�������
	}
	else if(Protocol4438->data[0] == 0x99 && Protocol4438->data[1] == 0x33)
	{
		SI443x_dataHandle();
		memset(Protocol4438,0,sizeof(Protocol433_t));
		shakeData_Analysis(); //��������������pwm
	}
	else
	{
		Si4438_Rcecv_Buff();//�������ݴ���
		Reoil_Cont++;
		if(Reoil_Cont>2000)
		{
			Reoil_Cont=0;
			connectstas=0;
		}
	}
}

/**
  * @brief :������ 
  * @param :��
  * @note  :��
  * @retval:��
***/ 
int main( void )
{
	delay_init( );																	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
														
	Power_Init();//���ӵ�Դ�����ߵ�Դ��ʼ��
	delay_ms(1000);
	uart_init( 115200 );	//����3��ʼ�� ����ʹ��
	Initial_UART2(115200);
	delay_ms(1000);		
	Led_Init();
	delay_ms(1000);		
	POWER_PIN_STATE=1;															//�ܳɰ��ϵ�
	WHITE_LED_ON;																		//�����ɹ�
	
	GFLASH.Read(0,(u8*)&flash_config,sizeof(flash_config));//��ȡ��������
	if(flash_config.pair_device.pair_ch != 0xff)
		Si4438ChannelSet=flash_config.pair_device.pair_ch;
	else
		Si4438ChannelSet=0;
	printf("Si4438ChannelSet=%d\r\n",Si4438ChannelSet);
	
	SI485_POWER_PIN_STATE=0;												//485ͨ��
	SI4438_POWER_PIN_STATE=0;												//4438ͨ��
	delay_ms(1000);																//�ȴ�4438���
	SI446x_Init();																	//SI4438��ʼ��
	pP_Instance = SendProtocolGetObj();							//ʵ����Э���
	//��ʼ��pwm
    Timer1_Init(TIM_arr, TIM_psc);
    Timer3_Init(TIM_arr, TIM_psc);
    Timer4_Init(TIM_arr, TIM_psc);
	//��ʼ����ʱ����������ʱ����
	Timer2_Init(TIM_arr, TIM_psc);
    srand(113);
	
	while(1)
	{
		Gun_Task();																		//Э���װ
		RF_SI4438_Ack_Cmd_Data();											//����Ӧ��

		if(Power_Shut_Down(30))//����3S�ػ�
		{
			delay_ms(100);
		}
		else if(connectstas)
		{
			BLUE_LED_ON;
		}
		else
		{
			WHITE_LED_ON;
		}
	}
}

