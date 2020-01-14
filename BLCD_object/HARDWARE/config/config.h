#ifndef _CONFIG_H_	
#define _CONFIG_H_

#ifdef __cplusplus		   //�����CPP����C���� //��ʼ����
extern "C" {
#endif

#include <math.h>
#include <string.h>			//�ַ����Ƚ�
#include <ctype.h>			//��дת��
#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx.h"		//STM32�̼���
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
	
//#include "bsp.h"			//�弶��ʼ��
//#include "static_init.h"	//printf
#include "GPIO_Init.h"
//#include "BKP_Init.h"
//#include "ADC.h"   
//#include "Time2.h"

#include "BLDCTIM_Init.h" 


//#include "usb_lib.h"
//#include "hw_config.h"
//#include "usb_pwr.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
typedef enum
{
  CW=0,  // ˳ʱ�뷽��(���ŵ���ῴ)��һ�㶨��Ϊ��ת
  CCW=1  // ��ʱ�뷽��(���ŵ���ῴ)��һ�㶨��Ϊ��ת
}MOTOR_DIR;

typedef enum 
{
  STOP=0,  // ͣ��
  RUN=1    // ����
}MOTOR_STATE;

typedef struct
{
  __IO int          motor_speed;        // ���ת��(RPM):0..3500
  __IO MOTOR_STATE  motor_state;        // �����ת����
  __IO MOTOR_DIR    motor_direction;    // ���ת������
  __IO uint32_t     step_counter;       // �������������������ڲ������ת��
  __IO uint16_t     stalling_count;     // ֹͣ��־�����ֵ����2000����Ϊ���ֹͣ��ת
}MOTOR_DEVICE;
extern MOTOR_DEVICE bldc_dev;
//����PID�ṹ��
typedef struct 
{
   __IO int      SetPoint;      //�趨Ŀ�� Desired Value
   __IO double   Proportion;    //�������� Proportional Const
   __IO double   Integral;      //���ֳ��� Integral Const
   __IO double   Derivative;    //΢�ֳ��� Derivative Const
   __IO int      LastError;     //Error[-1]
   __IO int      PrevError;     //Error[-2]
}PID;

/* ˽�к궨�� ----------------------------------------------------------------*/
/*************************************/
//����PID��غ�
//�����������趨�Ե������Ӱ��ǳ���
/*************************************/
#define  P_DATA                   0.5               //P����
#define  I_DATA                   0.06              //I����
#define  D_DATA                   0                 //D����

//#define  MOTOR_RATED_SPEED        2500              //ȷ��ת��
#define  MOTOR_MAX_SPEED          10000              //����ת��
#define  MOTOR_MIN_SPEED          100               
#define  MOTOR_POLE_PAIR_NUM      4                 //������(8 �ż���)



/************************************/
#define LED_RED_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_9 );  	   //LED1 
#define LED_RED_ON()  		GPIO_ResetBits(GPIOB, GPIO_Pin_9 ); 	  

#define LED_GREEN_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_8 );  	   //LED2 
#define LED_GREEN_ON()  	GPIO_ResetBits(GPIOB, GPIO_Pin_8 ); 	  

#define Ctrl_Brake_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_12 );  	  
#define Ctrl_Brake_ON()  	GPIO_ResetBits(GPIOB, GPIO_Pin_12 ); 	  


extern const unsigned char Ascill_16[];

#define Enable_PrintfDebug

void DebugPrintf(const char *fmt,...);
void TIM1_PWM_Init(void);


#ifdef __cplusplus		   //�����CPP����C���� //��������
}

#endif

#endif
