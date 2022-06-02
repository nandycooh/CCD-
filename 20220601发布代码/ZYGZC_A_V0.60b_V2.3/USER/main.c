#include "delay.h"
#include "drv_spi.h"
#include "drv_SI4438.h"
#include "FreeRTOS.h"
#include "task.h"
#include "power.h"
#include "usr_c322.h"
#include "led.h"
#include "hal_timer.h"
#include "attitude.h"
#include "motor.h"
#include "usart.h"	
#include "emserial.h"

#define START_TASK_PRIO		1							//�������ȼ� Խ�����ȼ�Խ�ߣ����ж����ȼ��෴��
#define START_STK_SIZE 		128  					//�����ջ��С ��Ϊ��u16�����д�СӦ����256���ֽ�
TaskHandle_t StartTask_Handler;					//������
void start_task(void *pvParameters);		//������

#define WIFIRECVE_TASK_PRIO		2					//�������ȼ�
#define WIFIRECVE_STK_SIZE 		128				//�����ջ��С  
TaskHandle_t WIFIRECVETask_Handler;			//������

void wifirecve_task(void *pvParameters);//������
#define MASTER_TASK_PRIO		3						//�������ȼ� 
#define MASTER_STK_SIZE 		128					//�����ջ��С  
TaskHandle_t MASTERTask_Handler;				//������
void master_task(void *pvParameters);		//������

#define SI433ACK_TASK_PRIO		4					//�������ȼ�
#define SI433ACK_STK_SIZE 		128				//�����ջ��С  
TaskHandle_t SI433Task_Handler;					//������
void si433ack_task(void *pvParameters);	//������

extern u8 rx_buf[RX_NUM_SIZE];
extern HalAdapter_Management_FLASH_t GFLASH;
extern flash_config_t flash_config;
extern HalAdapter_Management_Led_t halAdapter_Led;//API�ӿڹ��ⲿʹ��
extern DataPayload_t dataPayload;

void Restart(void)
{
//	__disable_irq();
//	NVIC_SystemReset();
	__set_FAULTMASK(1);//�ر����ж�
	NVIC_SystemReset();//����Ƭ������
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    															//��ʱ������ʼ�� ����2
	uart_init(115200);															//����ʹ��
	Power_Init();																		//��Դ��ʼ��
	halAdapter_Led.HAL_LedInit();										//��ʼ��LED
	RS232_Init(921600);															//340��ʼ��	 ����1	
	RS232_POWER_STATE			= 0;											//340ͨ�� ���߶Ͽ�
	delay_xms(100);
	RS232_RESET_STATU =1;
	
	SI4438_POWER_PIN_STATE=0;												//4438ͨ��

	Get_Mcu_Mac();
	
	GFLASH.Read(0,(u8*)&flash_config,sizeof(flash_config));//��ȡ��������
	if(flash_config.pair_device.pair_ch != 0)
		dataPayload.nrf_channel=flash_config.pair_device.pair_ch;
	else
		dataPayload.nrf_channel=0;
	SI446x_Init();																			//4438��ʼ��
	
	GREEN_LED_ON;
	delay_xms(1000);

	//��ʼ���ɹ�
	//������ʼ����
	  xTaskCreate((TaskFunction_t )start_task,            //������
				  (const char*    )"start_task",          //��������
				  (uint16_t       )START_STK_SIZE,        //�����ջ��С
				  (void*          )NULL,                  //���ݸ��������Ĳ���
				  (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
				  (TaskHandle_t*  )&StartTask_Handler);   //������              
	  vTaskStartScheduler();          										//�����������
}

//��ʼ�����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           										//�����ٽ���
    //��������
    xTaskCreate((TaskFunction_t )master_task,  					//������   	
                (const char*    )"master_task", 				//��������  	
                (uint16_t       )MASTER_STK_SIZE, 			//�����ջ��С
                (void*          )NULL,									//���ݸ��������Ĳ���
                (UBaseType_t    )MASTER_TASK_PRIO,			//�������ȼ�
                (TaskHandle_t*  )&MASTERTask_Handler); 	//������
		 xTaskCreate((TaskFunction_t )wifirecve_task,     	
                (const char*    )"wifirecve_task",   	
                (uint16_t       )WIFIRECVE_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )WIFIRECVE_TASK_PRIO,	
                (TaskHandle_t*  )&WIFIRECVETask_Handler);					
		xTaskCreate((TaskFunction_t )si433ack_task,     	
                (const char*    )"si433ack_task",   	
                (uint16_t       )SI433ACK_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )SI433ACK_TASK_PRIO,	
                (TaskHandle_t*  )&SI433Task_Handler);
		vTaskSuspend(SI433Task_Handler);										//������Э������
    vTaskDelete(StartTask_Handler); 										//ɾ����ʼ����
    taskEXIT_CRITICAL();           	 										//�˳��ٽ���
}

void si433ack_task(void *pvParameters)
{	
	static u8 i=0;
	static u8 Ackcountflg;

	while(1)
  { 
		taskENTER_CRITICAL();//�����ٽ�������
		if(Ackcountflg++ == 2)
		{
			i++;
			Send_Master_Cmd_Data(i);//��ѯ��������
	
			if(i>=7)
				i=0;
	
			Ackcountflg=0;
		}
		else
			Si433_Rcecv_Buff();		//���ݽ��մ���
		
		taskEXIT_CRITICAL(); //�˳��ٽ������� 	
		vTaskDelay(5);
  }
}

//��Э�������� 
void master_task(void *pvParameters)
{
	static u16 temp_cnt = 0;
	
	if(Login_Or_Boardcast(1))//����λ����������
	{
		vTaskResume(SI433Task_Handler );//�ָ���Э������
	}
				
	while(1)
	{
		/*��Э�飺��̬�ϴ�*/
		attitudeUploadHandler();
			
		/*�����ϴ�*/
		if(++temp_cnt >= BAT_UPDATA_TIME)
		{
			temp_cnt = 0;
			protocol_sendData(GetbatteryPresent(),5,BAT_DATA_CMD);
		}

		vTaskDelay(35);//30ms������Э��һ�Σ����ܹ���
	}
}  

void wifirecve_task(void *pvParameters)
{
    while(1)
    {
		taskENTER_CRITICAL();//�����ٽ�������
		DataCallBack_WIFI(rx_buf,sizeof(rx_buf));
		taskEXIT_CRITICAL(); //�˳��ٽ�������
		vTaskDelay(5);
    }
}  



