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

/*“config”开始的宏和“INCLUDE_”开始的宏一样，都是用来完成 FreeRTOS 的配置和裁剪的*/

#define configUSE_PREEMPTION				1															//1使用抢占式内核，0使用协程
#define configUSE_IDLE_HOOK					0															//1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK					0															//1，使用时间片钩子；0，不使用
#define configCPU_CLOCK_HZ					( ( unsigned long ) 72000000 )//设置 CPU 的频率	
#define configTICK_RATE_HZ					( ( TickType_t ) 1000 )				//时钟节拍频率，这里设置为1000，周期就是1ms
#define configMAX_PRIORITIES				( 5 )													//可使用的最大优先级 configMAX_PRIORITIES-1 的优先级最高
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )		//空闲任务使用的堆栈大小
#define configTOTAL_HEAP_SIZE				( ( size_t ) ( 17 * 1024 ) )	//系统所有总的堆大小
#define configMAX_TASK_NAME_LEN			( 16 )												//任务名字字符串长度	
#define configUSE_TRACE_FACILITY		0															//为1启用可视化跟踪调试
#define configUSE_16_BIT_TICKS			0															//系统节拍计数器变量数据类型，
																																	//1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD			1															//为1时空闲任务放弃CPU使用权给其他同优先级的用户任务

/* Co-routine definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS与协程有关的配置选项                                                  */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 		0//为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )//协程的有效优先级数目

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1//如果为1，vTaskPrioritySet才能被使用
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255//内核中断优先级 用来设置 PendSV 和滴答定时器的中断优先级

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /*系统可管理的最高中断优先级 equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15 //用来设置最低优先级

/***********************************************************************/
/*                FreeRTOS与中断服务函数有关的配置选项                 */
/**********************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler

//////210617 开启RTOS定时器时定时器时定义
////配置文件主要集中在FreeRTOS.h 和FreeRTOSConfig.h
//#define configTIMER_TASK_PRIORITY 1
//#define configTIMER_QUEUE_LENGTH 1
//#define configTIMER_TASK_STACK_DEPTH 1

#endif /* FREERTOS_CONFIG_H */

