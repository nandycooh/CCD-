#include "usr_c322.h"
#include "attitude.h"
#include "FreeRTOS.h"
#include "task.h"
#include "attitude.h"

static u8 rcv_byte = 0;
static u8 rcv_byte_last = 0;
static int rx_num = 0;
u8 rx_buf[RX_NUM_SIZE];//����3����buff
u8 rx_len=0;
static u8 Version[48];
u8 BoardConnectFlg = 0;/*��ǰ���������״̬*/
u8 NewChannel;
BroadCastData_t BroadCastData;/*�㲥�����ϴ�*/
LoginOrBoardCastAckData_t  LoginOrBoardCastAckData;/*�㲥���ߵ�¼��������*/
funtionFlag_t  funtionFlag;/*���ܱ�־λ*/

extern MacDataDef Macstruct;
extern HalAdapter_Management_FLASH_t GFLASH;
extern flash_config_t flash_config;
extern HalAdapter_Management_Led_t halAdapter_Led;//API�ӿڹ��ⲿʹ��
extern TaskHandle_t SI433Task_Handler;					//������
extern DataPayload_t dataPayload;
extern Protocol433_t Protocol433;

extern void Restart(void);

/***********************************************************************************************************************************
																											Ӳ��������
***********************************************************************************************************************************/
/************************************************************  	
�������ƣ�	Use_C322_Init
�������ܣ�	WIFI ���ų�ʼ��
��ڲ�����	bound��������
���ز�����	��
˵����	    ��
***********************************************************/ 
void RS232_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1��GPIOAʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2

	//USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3  
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���2
}

/************************************************************  	
�������ƣ�	Usart_SendByte
�������ܣ�	����һ���ֽ�
��ڲ�����	void
���ز�����	��
˵����	    ��
***********************************************************/ 
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/************************************************************  	
�������ƣ�	Usart_SendString
�������ܣ�	�����ַ���
��ڲ�����	void
���ز�����	��
˵����	    ��
***********************************************************/ 
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/************************************************************  	
�������ƣ�	usartx_puchar
�������ܣ�	�����ַ���
��ڲ�����	USARTX    : USART1 | USART2| USART3   (ѡ�񴮿�)
            buff      :"sadsad"| str              (�ַ����������׵�ַ)
            len       :strlen(str)                (�ַ��������鳤��)
���ز�����	��
˵����	    ��
***********************************************************/ 
void usartx_puchar(USART_TypeDef * USARTX,u8 *buff, u32 len)
{  
	USARTX->SR &=~(1<<6);
	while(len--)
	{
		USARTX->DR = *(buff++);
		while(!(USARTX->SR & (1<<6)));
	}
}


void USART1_IRQHandler(void)//210417
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		rcv_byte_last = rcv_byte;//��һ�ν����ֽ�
		rcv_byte =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		rx_buf[rx_num++] = rcv_byte;
		
		if(rcv_byte_last==0x99 && rcv_byte==0x33)//������λ���·�Э��
		{
		  rx_buf[0] = rcv_byte_last;
		  rx_buf[1] = rcv_byte;
		  rx_num    = 2;			
		}
		
		//����ÿ�ν������ݳ����жϸ������Ƿ�������
		rx_len=rx_buf[8]+0x0C;//���ݳ���+12�ֽ��������ݣ�ͷ+�汾��+�����+ACK+���ݳ���1�ֽ�+����+CRC����
//		if(rx_num >= RX_NUM_SIZE || (rx_buf[rx_num-2] == 0x0D && rx_buf[rx_num-1] == 0x0A) || rx_num>=rx_len) //��λ���·�֡��Ϊ12�ֽ�
//    {       
////			rcv_byte=0;
//			rx_num = 0;
//			rx_len=0;
//    }
	
			if( rx_num>=rx_len) //��λ���·�֡��Ϊ12�ֽ�
			{       
		//			rcv_byte=0;
					rx_num = 0;
					rx_len=0;
			}
	
	}
}

/***********************************************************************************************************************************
																											  ����ģ���
***********************************************************************************************************************************/

//������ڽ��յ��ַ���
void WIFI_Clear(void)
{
	memset(rx_buf, 0, sizeof(rx_buf));
	rx_num = 0;
}

//	��ڲ�����	cmd������
//	res����Ҫ���ķ���ָ��
//	���ز�����	1-�ɹ�	0-ʧ��
u8 WIFI_Send_Cmd_Check(u8 *cmd, u8 *res)
{
	unsigned char timeOut = 50;
	WIFI_Clear();
	
	usartx_puchar(USART2, (u8*)cmd, strlen((const char *)cmd));   //����2����ATָ���

	while(timeOut--)
	{
		if(rx_num > 0)
		{						             	
//			usartx_puchar(USART3, (u8*)rx_buf, strlen((const char *)rx_buf));//����յ�����
			if(strstr((const char *)rx_buf, (const char *)res) != NULL)
			{								                      //��ջ���			
				return 1;
			}
			WIFI_Clear();	
		}
		delay_ms(10);
	}
		 
	return 0;
}


//wifi���յ������Ƿ�Ϊ��
//ptr�����ص�����
//		0-У��ʧ��
int WIFI_Cmd_Check(char *ptr)
{
	if(strstr((char*)rx_buf,ptr)!= NULL)
		return 1;
	else 
		return 0;
}

/**
  * @brief ����������
  * @param
  * @retval
**/
//void HeartCheckCallBack(void)
//{	
//	if(BoardConnectFlg==1)//����״̬
//	{
//		Rebootcnt=0;
//		BoardConnectFlg=0;
//		BLUE_LED_ON;
//	}
//	else
//	{
//		Rebootcnt++;
//		//1�������׵�
//		if(Rebootcnt >= 30*100)
//		{
//			WHITE_LED_ON;
//			/*������ӵ�����״̬*/
//			BoardConnectFlg = 0;
//		}
//	}
//}

/**
  * @brief ��ȡ��Ƭ��MAC��ַ
  * @param
  * @retval: None
*/
u8* Get_MCU_MAC(void)
{
	static u8 ID[12];
	static u32 UID_ADDR = 0x1FFFF7E8;
	for(u8 i=0;i<12;i++) {
		 ID[i] = (*(__IO u32 *)(UID_ADDR+(i/4)*4) >>((i%4)*8))&0x000000ff;
	}
	return ID;
}
/**
  * @brief wifi mac ��ȡ
  * @param
  * @retval
**/
uint8_t* Wifi_Get_Mac(void)
{
	static uint8_t MAC[6];
	unsigned char decrypt[16];
	MD5_CTX md5;
  MD5Init(&md5);
	u8* pMac = Get_MCU_MAC();
	MD5Update(&md5,pMac,strlen((char *)pMac));
	MD5Final(&md5,decrypt);
	
	//16λMD5
	memcpy(MAC,&decrypt[4],6);
	
	return MAC;
}
extern SLAVE_VERSION_MSG SLAVE_VERSION_MSG_Struct;
u8* GetVerison(void){
	int i = 0;
	//�ܳɰ�
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
	Version[i++] = 	 BYTE0(temp);
	//�Ų�
	temp = SLAVE_VERSION_MSG_Struct.Foot_Hardware_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Foot_Soft_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Foot_Protocol_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	//��ǹ
	temp = SLAVE_VERSION_MSG_Struct.Gun_Hardware_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Gun_Soft_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Gun_Protocol_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	//����
	temp = SLAVE_VERSION_MSG_Struct.Back_Hardware_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Back_Soft_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i++] = 	 BYTE0(temp);
	temp = SLAVE_VERSION_MSG_Struct.Back_Protocol_Version;
	Version[i++] = BYTE3(temp);
	Version[i++] = BYTE2(temp);
	Version[i++] = BYTE1(temp);
	Version[i] = 	 BYTE0(temp);
	return Version;
}

/***********************************************************************************************************************************
																											  Ӧ�ò�
***********************************************************************************************************************************/
void Get_Mcu_Mac(void)
{
	/*�õ�ģ���MAC��ַ �͵õ��������IP��ַ*/
	memcpy(BroadCastData.MAC,Wifi_Get_Mac(),6);//��ȡ�豸ID

	memcpy(Macstruct.MAC,BroadCastData.MAC,6);//�����豸ID 210708
	
	/*�õ�wifi��IP�Լ�MAC��ַ*/
	BroadCastData.port = CLIENT_PORT;
	
	debug_printf_hex(16,"MAC:"," ",6,BroadCastData.MAC,"\r\n");
}

/**
  * @brief 	����λ����������
  * @param  
  * @retval
**/
u8 Login_Or_Boardcast(uint8_t BoardcastMode)
{
	/*�ȴ���λ����Ӧ��*/
	while(!funtionFlag.IsLoginOrBoardCastAck)//��λ��Ӧ���־ ��Ӧ��һֱ�ȴ�
	{
		/*��¼ʱ��IP��ַ�Լ�MAC  ���߰汾��IP*/
		protocol_sendData((u8*)&BroadCastData,sizeof(BroadCastData),LOGIN_DATA_CMD);
		
		halAdapter_Led.HAL_LedBlueTurn();

		delay_ms(BOARD_CAST_CYCLE);//�㲥ʱ����
	}
	
	/*���浽flash��*/
	/*��λ�� ��������ȷIP��ַ*/
	memcpy(flash_config.servercofig.port,SERVER_PORT,sizeof(SERVER_PORT));
	
	GFLASH.Write(0,(u8*)&flash_config,sizeof(flash_config));
	
	return 1;
}

/**
  * @brief  wifi_DataCallBack ���ݻص�����
  * @param
  * @retval
**/
void DataCallBack_WIFI(uint8_t* Data,uint16_t Len)
{
	static u16 crc16;
	static u8 receiveBuf[128];
	static u8 *p = NULL;
	if(Len < 10)
		return ;
	u16 receiveLen = 0;
	
	/*������λ�����ص���Ϣ*/
	/*�����ж�֡ͷ*/
	if(Data[0] == 0x99 && Data[1] == 0x33)
	{
		Len=Data[8]+0x0C;//���ݳ���+12�ֽ��������ݣ�ͷ+�汾��+�����+ACD+���ݳ���1�ֽ�+����+CRC��
//		debug_printf_hex(16,"cmd:"," ",Len,Data,"\r\n");
		/*CRCУ��*/
		/*CRCһ��Ҫ��*/
		crc16= Message_count_CRC_2(Len-2,Data);
		p = (u8*)&crc16;
		if(Data[Len-1]!= p[1] && Data[Len-2] != p[0])
			return;
		/*�õ����ݳ���*/
		receiveLen = Data[8];
		/*��ȡ������*/
		if(receiveLen < 128)
			memcpy(receiveBuf,Data+10,receiveLen);

		/*�ж�����*/
		switch(Data[9])
		{
			case 0x01://��������������				
				if(funtionFlag.IsWorkMode == 0)
				{
					/*�㲥���ػ��ߵ�¼����,���������·���¼����Կ�Լ�IP��MAC*/
					funtionFlag.IsLoginOrBoardCastAck = 0x01;
					
					memcpy((u8*)&LoginOrBoardCastAckData,receiveBuf,receiveLen);
					/*����ģʽ��־λֱλ*/
					funtionFlag.IsWorkMode = 1;
				}
				/*����״̬*/
				BoardConnectFlg = 1;
				
				funtionFlag.IsWorkMode = 1;
//				/*��������������*/
//				Rebootcnt = 0;	
				break;
			case 0x05:
				vTaskSuspend(SI433Task_Handler);
				delay_xms(50);
				/*���ĵ������(������)*/
				SI446x_Send_Packet(dataPayload.nrf_channel, 0x80,(uint8_t *)Data, Len);
				delay_xms(50);
				vTaskResume(SI433Task_Handler );	
				break;			
			case 0x06:
				/*�㲥���ػ��ߵ�¼����,���������·���¼����Կ�Լ�IP��MAC*/
				funtionFlag.IsLoginOrBoardCastAck = 0x01;
				
				memcpy((u8*)&LoginOrBoardCastAckData,receiveBuf,receiveLen);
				/*����ģʽ��־λֱλ*/
				funtionFlag.IsWorkMode = 1;BLUE_LED_ON;
				break;
			case 0x07://�����ŵ�
				funtionFlag.IsSetChannel=true;
			
				if(receiveBuf[0]<255)
					Protocol433.data[4]=receiveBuf[0];
				if(receiveBuf[2]<255)
					Protocol433.data[5]=receiveBuf[2];
				if(receiveBuf[3]<255)
					Protocol433.data[6]=receiveBuf[3];
				if(receiveBuf[4]<255)
					Protocol433.data[7]=receiveBuf[4];
				if(receiveBuf[1]<255)
					NewChannel=receiveBuf[1];//�õ��ŵ�
				
//				printf("->> %x %d %d %d %d\r\n",Protocol433.data[4],NewChannel,Protocol433.data[5],Protocol433.data[6],Protocol433.data[7]);
				break;
			case 0x08://�����豸
				
				if(receiveBuf[10] == 0x01){
					Restart();//�ܳɰ�����
				}
				else if(receiveBuf[10] == 0x00){
					protocol_sendData(GetVerison(),48,VERSION_DATA_CMD);
				}
				else if(receiveBuf[10] == 0x03){
					Protocol433.data[13]=0x03;
				}
				else if(receiveBuf[10] == 0x02){
					Protocol433.data[12]=0x01;//�����Ų��Զ��ػ�
				}
				else if(receiveBuf[10] == 0x04){
					Protocol433.data[12]=0x00;//�رսŲ��Զ��ػ�
				}
				printf("Usart recv:%x %x\r\n",receiveBuf[10],Protocol433.data[8]);
				memset(receiveBuf,0,sizeof(receiveBuf));
				memset(&SLAVE_VERSION_MSG_Struct,0,sizeof(SLAVE_VERSION_MSG_Struct));
				break;
			default:break;
		}		
	}

	memset(rx_buf,0,sizeof(rx_buf));//210930
}

