#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"

#define v0_44
struct shakeType
{
    uint16_t shakeStrength;//������
    uint16_t shakeFreq;//��Ƶ��
    uint16_t shakeTime;//
    uint8_t  shakeMode;//
};
extern struct shakeType
Head, Occiput, Chest_L, Chest_R, Rib_L, Rib_R, Belly_L, Belly_R,
 Back_L, Back_R, Waist_L, Waist_R, Arm_L, Wrist_L, Hand_L, 
 Arm_R, Wrist_R, Hand_R, Thigh_L, Shank_L, Foot_L, 
 Thigh_R, Shank_R, Foot_R, Shoulder_L, Shoulder_R;

#define SHOULDER        //�粿
#define CHEST           //�ز�/�粿
//#define RIB             //�߲�
//#define BELLY           //����/��
#define BACK            //����
//#define WAIST           //����    
//#define ARM             //��֫
//#define THIGH           //��֫

#ifdef v0_34
#define CHEST_L_TIM     TIM2->CCR1
#define CHEST_R_TIM     TIM2->CCR2
#define RIB_L_TIM       TIM4->CCR1
#define RIB_R_TIM       TIM4->CCR2
#define BELLY_L_TIM     TIM2->CCR4
#define BELLY_R_TIM     TIM2->CCR3
#define BACK_L_TIM      TIM1->CCR4
#define BACK_R_TIM      TIM1->CCR2
#define WAIST_L_TIM     TIM1->CCR3
#define WAIST_R_TIM     TIM1->CCR1
#endif

#ifdef v0_40
#define CHEST_L_TIM     TIM1->CCR2     //�粿
#define CHEST_R_TIM     TIM2->CCR2
#define BELLY_L_TIM     TIM1->CCR1     //����
#define BELLY_R_TIM     TIM2->CCR1
#define BACK_L_TIM      TIM1->CCR3     //����
#define BACK_R_TIM      TIM2->CCR3     
#define WAIST_L_TIM     TIM1->CCR4     //����
#define WAIST_R_TIM     TIM2->CCR4
#endif


#ifdef v0_40_1

#define CHEST_L_TIM    TIM1->CCR2       //�粿
#define CHEST_R_TIM    TIM1->CCR4       //�粿
#define BELLY_L_TIM    TIM1->CCR1       //��
#define BELLY_R_TIM    TIM2->CCR1       //��
#define BACK_L_TIM     TIM1->CCR3       //��
#define BACK_R_TIM     TIM2->CCR2

#endif

#ifdef v0_42

/*
G��    ��        ��       B��  ��         ��   
       ��1       ��5           ��9        ��10
       ��2       ��6           ��2        ��6
       ��3       ��7                 ��3
       ��4       ��8
       1-4��Timer1    5-8��Timer2     9-10��Timer4
*/


#define SHOULDER_L_TIM    TIM1->CCR2     //�粿  
#define SHOULDER_R_TIM    TIM2->CCR2
#define CHEST_L_TIM       TIM4->CCR1     //�ز�
#define CHEST_R_TIM       TIM4->CCR2
#define BELLY_L_TIM       TIM1->CCR1     //����
#define BELLY_R_TIM       TIM2->CCR1
#define BACK_L_TIM        TIM1->CCR3     //����
#define BACK_R_TIM        TIM2->CCR3     
#define WAIST_L_TIM       TIM1->CCR4     //����
#define WAIST_R_TIM       TIM2->CCR4

#endif

#ifdef v0_43

#define SHOULDER_L_TIM    TIM4->CCR1     //�粿  
#define SHOULDER_R_TIM    TIM1->CCR3
#define CHEST_L_TIM       TIM3->CCR1     //�ز�
#define CHEST_R_TIM       TIM4->CCR2
#define BACK_L_TIM        TIM1->CCR2     //����

#endif
#ifdef v0_44

#define SHOULDER_L_TIM    TIM1->CCR3   
#define SHOULDER_R_TIM    TIM3->CCR1  //�粿
#define CHEST_L_TIM       TIM4->CCR2
#define CHEST_R_TIM       TIM4->CCR1     //�ز�
#define BACK_L_TIM        TIM1->CCR2     //����
//#define BACK_R_TIM        TIM2->CCR3     

#endif

#endif
