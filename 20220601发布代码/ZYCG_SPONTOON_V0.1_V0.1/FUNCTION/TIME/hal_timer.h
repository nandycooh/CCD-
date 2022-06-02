#ifndef __HAL_TIMER_H
#define __HAL_TIMER_H

#include "sys.h"


/*���Ʊ�־λ*/
typedef struct
{
	bool is_UpDateLED;//���ݸ��±�־λ
	bool is_ConnectTerminal;//���ӱ�־λ
	bool is_ConnectChargingInterface;//���ӳ������־λ
	bool is_LowBattery;//�͵�����־λ
//	bool is_WaitingStartUp;//˯�߱�־λ
	bool is_RfClearReceiveBuffer;//���BUFFER
	bool is_TurnOffFlag;
}Event_Flag_t;

//extern Event_Flag_t p_Flag;

/*��ʱ������API
*/
typedef struct
{
	void (*start)(u8 timerEx_id,uint32_t timer_period);
	void (*stop)(u8 timerEx_id);
}TIME_ControlDef;

void timer_start(u8 timerEx_id,uint32_t timer_period);
void TIM3_Int_Init(u16 arr,u16 psc);
void ApplicationTimer_Stop(void);

#endif
