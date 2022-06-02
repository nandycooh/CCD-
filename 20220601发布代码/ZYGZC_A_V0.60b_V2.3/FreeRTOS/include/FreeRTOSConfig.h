/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/*��config����ʼ�ĺ�͡�INCLUDE_����ʼ�ĺ�һ��������������� FreeRTOS �����úͲü���*/

#define configUSE_PREEMPTION				1															//1ʹ����ռʽ�ںˣ�0ʹ��Э��
#define configUSE_IDLE_HOOK					0															//1��ʹ�ÿ��й��ӣ�0����ʹ��
#define configUSE_TICK_HOOK					0															//1��ʹ��ʱ��Ƭ���ӣ�0����ʹ��
#define configCPU_CLOCK_HZ					( ( unsigned long ) 72000000 )//���� CPU ��Ƶ��	
#define configTICK_RATE_HZ					( ( TickType_t ) 1000 )				//ʱ�ӽ���Ƶ�ʣ���������Ϊ1000�����ھ���1ms
#define configMAX_PRIORITIES				( 5 )													//��ʹ�õ�������ȼ� configMAX_PRIORITIES-1 �����ȼ����
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )		//��������ʹ�õĶ�ջ��С
#define configTOTAL_HEAP_SIZE				( ( size_t ) ( 17 * 1024 ) )	//ϵͳ�����ܵĶѴ�С
#define configMAX_TASK_NAME_LEN			( 16 )												//���������ַ�������	
#define configUSE_TRACE_FACILITY		0															//Ϊ1���ÿ��ӻ����ٵ���
#define configUSE_16_BIT_TICKS			0															//ϵͳ���ļ����������������ͣ�
																																	//1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD			1															//Ϊ1ʱ�����������CPUʹ��Ȩ������ͬ���ȼ����û�����

/* Co-routine definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS��Э���йص�����ѡ��                                                  */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 		0//Ϊ1ʱ����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )//Э�̵���Ч���ȼ���Ŀ

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1//���Ϊ1��vTaskPrioritySet���ܱ�ʹ��
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255//�ں��ж����ȼ� �������� PendSV �͵δ�ʱ�����ж����ȼ�

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /*ϵͳ�ɹ��������ж����ȼ� equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15 //��������������ȼ�

/***********************************************************************/
/*                FreeRTOS���жϷ������йص�����ѡ��                 */
/**********************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler

//////210617 ����RTOS��ʱ��ʱ��ʱ��ʱ����
////�����ļ���Ҫ������FreeRTOS.h ��FreeRTOSConfig.h
//#define configTIMER_TASK_PRIORITY 1
//#define configTIMER_QUEUE_LENGTH 1
//#define configTIMER_TASK_STACK_DEPTH 1

#endif /* FREERTOS_CONFIG_H */

