#include "delay.h"
#include "usart.h"
#include "rgb_led.h"
#include "hal_spi.h"
#include "hal_adc.h"
#include "hal_timer.h"
#include "drv_SI4438.h"
#include "powerManage.h"
#include "protocol.h"
#include "hal_timer.h"
#include "walk.h"
#include "mpu.h"
#include "mpu_6050.h"
#include "sleep.h"
#include "hal_mpu.h"
#include "func_init.h"
#include "crc.h"
#include <stdio.h>
#include "math.h"
#include "hal_flash.h"

#define TEST_SI4438_FLAG
#define TEST_SLEEP_FLAG

HAL_adapter_t* pH_Instance;						//6050Ӳ����
FUNC_adapter_t* pF_Instance;					//MPU���ܲ�
ProtocolFormat_t* pP_Instance;					//Э���
VoidGear_Walk_t* pW_Instance;					//�Ų������㷨
Event_Flag_t p_Flag;							//��־λ

typedef enum
{
	ON = 0,
	OFF,
}Work_Status;

static uint8_t BatteryQuantity = 0;		//����
static uint8_t eliminationCount = 0;	//���߼���
extern int connetflg;					//δͨ�ż���
extern u8 dgbugcount;					//��Ϣ�������
extern u8 Low_Battery_Flg;

u8 Si4438ChannelSet=0;

//��Դ���͹���ģʽ�ṹ�弰ָ������
extern HalAdapter_Management_Power_t halAdapter_Power;
extern HalAdapter_Management_OperatingMode_t halAdapter_OptMode;
extern Msg_t p_Msg;//��������
extern LED_DRIVER_APP_API Led_Driver_App_Port;//API�ӿڹ��ⲿʹ��

/************************************************��ʱ������**************************************************/
//���ӿ�״̬�ж�
void UpdateChargingInterfaceStatus(void)
{
	static uint8_t chargingInterfaceStatus = 0;
	chargingInterfaceStatus <<= 1;
	if(halAdapter_Power.HAL_Get_ChargePinStatus() == true)//1�����ڲ���
	{
		chargingInterfaceStatus |= 1;
	} 
	else{
		chargingInterfaceStatus = 0;
	}
	
	if(chargingInterfaceStatus == 0xFF){
		p_Flag.is_ConnectChargingInterface = true;
	}
	else{
		p_Flag.is_ConnectChargingInterface = false;
	}
}

//�͵����ж�
void UpdateBatteryQuantity(void)
{
	static uint8_t BatteryTCount = 0;
	static uint8_t BatteryFCount = 0;				//���ڵ�����ȡ���ȶ���ʹ��������������210712
	
	BatteryQuantity = halAdapter_Power.HAL_Get_battery_power();//��ȡ��ص���
	
	if(BatteryQuantity <= 20)
	{
		BatteryTCount++;											//�͵��� ����
		BatteryFCount=0;
		
		if(BatteryTCount > 10)
		{
			printf("p_Flag.is_LowBattery = true\r\n");
			p_Flag.is_LowBattery = true;				//������
			if(BatteryQuantity == 1){
				POWER_ON_OFF=0;//�ػ�
			}
		}
	}
	else{
		BatteryTCount = 0;
		BatteryFCount++;											//�ǵ͵��� ����
		if(BatteryFCount > 10)
		{
			p_Flag.is_LowBattery = false;				//��ձ�־λ
		}
	}
}

//��ֹ�ػ�
void DormancyState_Judge(void)
{
	static uint32_t stateCount = 0;
	if(pW_Instance->GetWalk() == 0)					//��ȡ���б�־λ 0��ֹ 1�˶�
	{
		stateCount++;
		if(stateCount == 10*10*60)						//10���ӽ����� -->��ʱ��100ms
		{
//			p_Flag.is_WaitingStartUp = true;		//˯�߱�־λ
//			POWER_ON_OFF=0;
//			/*�ػ�*/
			if(p_Flag.is_TurnOffFlag==true)
				POWER_ON_OFF=0;
		}
	}
	else{
		if(++eliminationCount > 50){
			stateCount = 0;
//			p_Flag.is_WaitingStartUp = false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
/*�������*/
void Charge_Task(void)
{
	ApplicationTimer_Stop();
	delay_ms(200);
	pF_Instance->MPU->LpMotionInterrupt(MOTION_DETECTION);
	Adc_ShutDown();//����ADC
	GPIO_SetBits(GPIOA, GPIO_Pin_3);//4438��Դ��
	while(halAdapter_Power.HAL_Get_ChargePinStatus()==true)//�Ƿ��ڳ��
	{	
		if(halAdapter_Power.HAL_Get_FullBatteryPin_InCharge()==true)//�Ƿ���� �ж�оƬ��ʽ�������籣����أ�
		{
			GREEN_LED_ON;
			POWER_ON_OFF=0;
		}
		else
		{
			RED_LED_ON;
			halAdapter_OptMode.HAL_EnterStop();//����ֹͣģ
		}		
	}
//	p_Flag.is_ConnectChargingInterface = halAdapter_Power.HAL_Get_ChargePinStatus();//��ȡ�Ƿ��ڳ��IO״̬
}

/*LED����*/
void LED_Task(void)
{	
	if(p_Flag.is_UpDateLED == true)
	{
		if(Power_Shut_Down(30))//����3S�ػ�
		{
			delay_ms(100);
		}
		else if(p_Flag.is_LowBattery == true)
		{
//			printf("enter LowPower\r\n");
			if(Low_Battery_Flg>=10){
				Low_Battery_Flg=0;
				Led_Driver_App_Port.App_LedTurnState(RED);//red��˸ 1S/��
			}
		}
		else
		{
			Low_Battery_Flg=0;
			
		
			if(p_Flag.is_ConnectTerminal == true)//��������
			{
				BLUE_LED_ON;
			}
			else
			{
				WHITE_LED_ON;
			}
		}
	}
}

u8 SI433_ID=0x00;
u8 Batons_State;
/*���м�������*/
void Walk_Task(void){
	unsigned long timestamp;
	unsigned char sensors, more;
	short accel[3] = {0}, gyro[3] = {0};
	static uint8_t quat[4] = {0};
	
	
	if(TOUCH_GPIO_STATE)
	{
		GREEN_LED_ON;
		Batons_State |= 0x01; 
	}
	else
		Batons_State &= (~0x01);
	
	if(pW_Instance->GetWalk())
	{
		RED_LED_ON;
		Batons_State |= 0x02;
	}
	else
		Batons_State &= (~0x02);
	
	if(pF_Instance->MPU->isGenerateNewData()) 									//�ӼĴ�����ȡ�¶�
	{
		pF_Instance->get_timestamp(&timestamp);										//��ȡʱ��
		if(!pF_Instance->MPU->ReadFIFO(gyro, accel, &timestamp, &sensors, &more))//��FIFO��ȡһ�����ݰ�
		{	
			pW_Instance->Algorithm_Walk(gyro,accel);								//�����㷨
			/*�Զ���Э��*/
			pP_Instance->FmtHead();
			pP_Instance->FmtU8(SI433_ID);														//�豸ID
			pP_Instance->FmtHex(quat,4);														//��Ԫ��
			pP_Instance->FmtU8(Batons_State);												//����״̬
			pP_Instance->FmtU16((uint16_t)pW_Instance->GetSpeed());	//�����ٶ�
			pP_Instance->FmtU8(0);																	//����
			pP_Instance->FmtU8(BatteryQuantity);										//��ص���
			pP_Instance->FmtU8(0);
			pP_Instance->FmtU8(0);
			
			pP_Instance->FmtHex(GetVerison(),12);//220125
			
			pP_Instance->FmtCrc8();																	//У��λ
			pP_Instance->FmtU8(FORMAT_TAIL);												//����β֡
			p_Msg.length = pP_Instance->GetLength();
			memcpy(p_Msg.data,pP_Instance->GetBuffer(), p_Msg.length);
		}
	}
}
extern HalAdapter_Management_FLASH_t GFLASH;
extern flash_config_t flash_config;
/**********************************************************************
  * @brief :������ 
  * @param :��
  * @note  :��
  * @retval:��
*************************************************************************/ 
int main( void )
{
//	uint8_t batval = 0;															//�����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	halAdapter_Power.HAL_Power_Init();							//��粿�ֳ�ʼ��	
	delay_init( );//��ʱ��ʼ��
																	
	delay_ms(1000);
	uart_init(115200);															//���ڳ�ʼ�� ����ʹ��
	delay_ms(1000);
	
	delay_ms(1000);
	POWER_ON_OFF=1;
	Led_Init();																			//LED��ʼ��
	Touch_Gpio_Init();
	Adc_init();															//������ȡ��ʼ��
	pW_Instance = WalkGetObj();											//�����㷨ʵ����
	pW_Instance->Init(AL_SAMPLE_RATE);
	pP_Instance = SendProtocolGetObj();							//ʵ����Э���
	halAdapter_Clock.HAL_ClockInit();								//ʵ����Ӳ����
	halAdapter_Clock.isInit = true;
	pF_Instance = GetFuncAdapter();									//ʵ�������ܲ�
	pF_Instance->MPU->Init(AL_SAMPLE_RATE);					//��������
	
	TOUCH_GPIO_POWER=1;
	
	GFLASH.Read(0,(u8*)&flash_config,sizeof(flash_config));//��ȡ��������
	if(flash_config.pair_device.pair_ch != 0xff)
		Si4438ChannelSet=flash_config.pair_device.pair_ch;
	else
		Si4438ChannelSet=0;
	
	if(flash_config.pair_device.device_num == SPONTOON ||
		flash_config.pair_device.device_num == SHIELD  || flash_config.pair_device.device_num == PAPPER_SPRAY)
		SI433_ID=flash_config.pair_device.device_num;
	else
		SI433_ID=SPONTOON;
	
#ifdef	TEST_SI4438_FLAG
	SI446x_Init();
#endif
	
	timer_start(1,100);//��ʱ��2��ʼ��100ms ���ӿڡ��͵����͹ػ���ʱ
	timer_start(2,100);//��ʱ��3��ʼ��100ms ��Ϣ���������ʱ
	Vibrate_Gpio_Init();
	
	p_Flag.is_UpDateLED = true;
	
	while(1)
	{	
		if(p_Flag.is_ConnectChargingInterface == true)	//��ʱ������
		{
			Charge_Task();							/*���״̬����*/
		}
		else
		{
			Walk_Task();						/*����MPU����*/
#ifdef	TEST_SI4438_FLAG
			Si4438_Rcecv_Buff();
#endif
			LED_Task();
				
			if(dgbugcount>=20)//2S��һ��
			{
//				printf("%d\r\n",Batons_State);
//				
				dgbugcount=0;
				printf("Batons_State:%d-speed:%f-channel:%dR-%x\r\n",Batons_State,pW_Instance->GetSpeed(),Si4438ChannelSet,p_Flag.is_TurnOffFlag);
			}
		}
	}
}



