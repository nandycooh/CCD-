#ifndef __FUNC_INIT_H
#define __FUNC_INIT_H

#include "func_config.h"
#include "mpu.h"
#include "hal_iic.h"
#include "hal_mpu.h"


#define AL_SAMPLE_RATE 													300                 //MPU�Ĳ������ʣ�4~1000��
#define AL_IIR_RATE															282									//��ͨ�˲���Ƶ��(����MPU�����ʵ���)
/***************************RF*********************************/

#define LEFTWALKID		0x55
#define RIGHTWALKID		0x56
//#define RF433_ID        												LEFTWALKID								//0x55->left   0x56->right

							//0x55->left   0x56->right

/***************************MPU********************************/
//Low-power accel mode
#define MOTION_THRESHOLD												1000 								//�˶������ֵ��32~8160��
#define MOTION_DURATION													1										//1ms���˶�����ʱ��
#define MOTION_SAMPLING_RATE										1										//Low-power accel mode supports the following frequencies: 1.25Hz(1), 5Hz(<=5), 20Hz(<=20), 40Hz(else)
#define MOTION_DETECTION												MOTION_THRESHOLD,MOTION_DURATION,MOTION_SAMPLING_RATE

//Ӳ����
typedef struct
{
	bool isInit;
	HalAdapter_Management_IIC_t* _IIC;
	HalAdapter_Management_MPU_t* _MPU;
}HAL_adapter_t;

//���ܲ�
typedef struct
{
	bool isInit;
	FuncAdapter_Management_MPU_t* MPU;

	void (*delay_ms)(u16 ms);
	void (*delay_us)(u32 us);
	void (*get_timestamp)(unsigned long*);
}FUNC_adapter_t;


FUNC_adapter_t* GetFuncAdapter(void);

#endif
