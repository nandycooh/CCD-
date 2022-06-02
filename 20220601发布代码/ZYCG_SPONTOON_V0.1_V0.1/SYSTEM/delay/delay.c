#include "delay.h"


static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
volatile uint32_t g_ul_ms_ticks=0;	

			   
////��ʼ���ӳٺ���
////��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
////SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
////SYSCLK:ϵͳʱ��
//void delay_init()
//{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
//	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  

//	fac_ms=(u16)fac_us*1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����   
//}								    

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()	 
{
#if CLOCKTICKER
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	if (SysTick_Config(SystemCoreClock/100000))
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
#else 
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����
#endif	
}


#if CLOCKTICKER
	/**
  * @brief  ����ӳ�MS
  * @param  None
  * @retval : None
**/
static void delay_ms(u16 time)
{
  u16 i;
	if(time == 0x00)
		return;
	while(time--)
	{
		i=12000;
		while(i--);
	}
}

/**
  * @brief  ����ӳ�US
  * @param  None
  * @retval : None
**/
static void delay_us(u32 time)
{
  u16 i;
	if(time == 0x00)
		return;
	while(time--)
	{
		i=12;
		while(i--);
	}
}

static void TimeStamp_Increment(void)
{
	g_ul_ms_ticks++;
}

static void get_tick_count(unsigned long *count)
{
  count[0] = g_ul_ms_ticks;
}

#else 

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
static void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
static void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
}
#endif

//SysTick_Handler����
void TimeStamp_Increment(void)
{
	g_ul_ms_ticks++;
}

void get_tick_count(unsigned long *count)
{
  count[0] = g_ul_ms_ticks;
}


HalAdapter_Management_Clock_t halAdapter_Clock = 
{
//.isInit = false,
.HAL_ClockInit = delay_init,
.HAL_DelayUS = delay_us,
.HAL_DelayMS = delay_ms,
.HAL_Get_TickMS = get_tick_count,
};


/**
  * @brief Ӳ��Clockʵ���ҳ�ʼ��
  * @param
  * @retval: None
*/
static HalAdapter_Management_Clock_t* HAL_adapterInit_Clock(void)
{
//	if(halAdapter_Clock.isInit == false){
		//ʵ����
		halAdapter_Clock.HAL_ClockInit();
		halAdapter_Clock.isInit = true;
//	}
	return &halAdapter_Clock;
}

/**
  * @brief ��ȡӲ��ʱ��ʵ��
  * @param
  * @retval: None
*/
HalAdapter_Management_Clock_t* GetHalAdapter_Clock(void)
{
	return HAL_adapterInit_Clock();
}































