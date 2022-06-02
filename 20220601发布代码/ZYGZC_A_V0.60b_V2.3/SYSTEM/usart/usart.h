#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define debug_printf(fmt,...)     				printf("[app]"fmt,##__VA_ARGS__)
#define notitle_printf(fmt,...)   				printf(fmt,##__VA_ARGS__)
//#define internal_debug_printf(fmt,...) 	  printf("[internal]"fmt,##__VA_ARGS__)
//#define internal_debug_printf 	  xprintf

//����Ӳ������ 210202
#define UART_PORT					USART1
#define UART_PORT_CLK				RCC_APB2Periph_USART1
#define UART_PORT_AF				GPIO_Remap_USART1

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

void uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
uint8_t uart_rx_bytes( uint8_t* RxBuffer,USART_TypeDef *USARTX);
void debug_printf_hex(u8 m,char* title,char* s,u8 len,u8* d,char* end);
#endif


